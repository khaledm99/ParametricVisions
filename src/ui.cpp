#include "ui.h"
#include "imgui_internal.h"
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>

void UI::setWindow(GLFWwindow* w)
{
    window = w;
}
void UI::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    returnedCurves.resize(4);
}

bool UI::beginMainWindow()
{
    bool change = false;
    static bool firstLoop = true;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    //
    //
    // The following is lifted from the ImGui demo code
    //
    //
    bool *p_open;
    bool popen = true;
    p_open = &popen;
    bool opt_fullscreen = true;
    bool opt_padding = false;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            bool test;
            ImGui::MenuItem("Fullscreen", NULL, &test);
            ImGui::MenuItem("Padding", NULL, &test);
            ImGui::Separator();

            ImGui::Separator();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    return change;

}
void UI::endMainWindowAndRender()
{
    ImGui::End();
}
bool UI::showConfig()
{
    bool change = false;
    
    ImGui::Begin("Config");
    change |= ImGui::Checkbox("Toggle Wireframe", &wire);
    change |= ImGui::Checkbox("Show Surface", &showSurface);
    change |= ImGui::Checkbox("Show Curves", &showCurves);
    change |= ImGui::RadioButton("Perspective", &perspective, 0); ImGui::SameLine();
    change |= ImGui::RadioButton("Orthographic", &perspective, 1);
    ImGui::Separator();
    change |= ImGui::DragFloat("View Distance", &viewDistance, 0.1f);
    change |= ImGui::DragFloat("Azimuth", &pitch, 0.1f);
    change |= ImGui::DragFloat("Polar", &yaw, 0.1f);

    ImGui::Separator();
    // Adapted from Online ImGui Manual
    // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
    // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
    // stored in the object itself, etc.)
    const char* items[] = {"None"
                          ,"Ruled"
                          ,"Bilinear"
                          ,"Coons"
                          ,"Rotational Blend"
                          ,"Revolution"};

    static int item_current_idx = 0; // Here we store our selection data as an index.
    const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo("Select Surface", combo_preview_value, 0))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
            {
                item_current_idx = n;
                surfaceType = (Surface)n;
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    build = false;
    if (ImGui::Button("Build")) build = true;
    ImGui::Separator();

    static Surface st = NONE;
    switch(surfaceType) 
    {
        case RULED:
        {
            static int curve_a = 0; 
            const char* combo_preview_value = std::to_string(curve_a).c_str();
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                        {
                            curve_a = n;
                            returnedCurves[0] = n;
                        }
                            

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();


            }
            static int curve_b = 0; 
            combo_preview_value = std::to_string(curve_b).c_str();
            if (ImGui::BeginCombo("Select Curve 2", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_b == n);
                        if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                        {
                            curve_b = n;
                            returnedCurves[1] = n;
                            std::cout<<"setting curve b"<<std::endl;
                        }
                            

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

            break;
        case COONS:
            {
                static int p0 = 0; 
                const char* combo_preview_value = std::to_string(p0).c_str();
                if (ImGui::BeginCombo("Select P0", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (p0 == n);
                            if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                            {
                                p0 = n;
                                returnedCurves[0] = n;
                            }
                                

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();


                }
                static int p1 = 0; 
                combo_preview_value = std::to_string(p1).c_str();
                if (ImGui::BeginCombo("Select P1", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (p1 == n);
                            if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                            {
                                p1 = n;
                                returnedCurves[1] = n;
                            }
                                

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                static int q0 = 0; 
                combo_preview_value = std::to_string(q0).c_str();
                if (ImGui::BeginCombo("Select Q0", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (q0 == n);
                            if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                            {
                                q0 = n;
                                returnedCurves[2] = n;
                            }
                                

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();


                }
                static int q1 = 0; 
                combo_preview_value = std::to_string(q1).c_str();
                if (ImGui::BeginCombo("Select Q1", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (q1 == n);
                            if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                            {
                                q1 = n;
                                returnedCurves[3] = n;
                            }
                                

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                build |= ImGui::RadioButton("Ruled Surface 1", &coonsStep, 0); 
                build |= ImGui::RadioButton("Ruled Surface 2", &coonsStep, 1); 
                build |= ImGui::RadioButton("Bilinear Patch", &coonsStep, 2);
                build |= ImGui::RadioButton("R1 + R2", &coonsStep, 3);
                build |= ImGui::RadioButton("Coons Patch (R1 + R2 - B)", &coonsStep, 4);
            }
            break;
        case ROTATIONAL:
            {
            static int curve_a = 0; 
            const char* combo_preview_value = std::to_string(curve_a).c_str();
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                        {
                            curve_a = n;
                            returnedCurves[0] = n;
                        }
                            

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();


            }
            static int curve_b = 0; 
            combo_preview_value = std::to_string(curve_b).c_str();
            if (ImGui::BeginCombo("Select Curve 2", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_b == n);
                        if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                        {
                            curve_b = n;
                            returnedCurves[1] = n;
                        }
                            

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            }
            break;
        case REVOLUTION:
            {
            static int curve_a = 0; 
            const char* combo_preview_value = std::to_string(curve_a).c_str();
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(std::to_string(n).c_str(), is_selected))
                        {
                            curve_a = n;
                            returnedCurves[0] = n;
                        }
                            

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();


            }
            
        }
            break;
        case NONE:
        default:
            break;
    }

    // Change view direction. Button loop adapted from interactive ImGui Demo
    for(int i =0; i<4;i++)
    {
        if(i>0)
            ImGui::SameLine();
        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
        switch(i) 
        {
            case(0):
                //if(ImGui::Button("X")) viewDirection = i;
                break;
            case(1):
                //if(ImGui::Button("Y")) viewDirection = i;
                break;
            case(2):
                //if(ImGui::Button("Z")) viewDirection = i;
                break;
            case(3):
                //if(ImGui::Button("Ortho")) viewDirection = i;
                break;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::End();
    return change;
}
bool UI::showViewport(unsigned int texColBuf)
{
    bool change = false;
    ImGui::Begin("Viewport");
    viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Image((ImTextureID)texColBuf, viewportSize, ImVec2(0.f,1.f), ImVec2(1.f,0.f)); 
    ImGui::End();
    return change;
}
void UI::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImVec2 UI::getViewportSize()
{
    return viewportSize;
}

