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
    change |= ImGui::DragFloat("Azimuth", &pitch, 0.2f);
    change |= ImGui::DragFloat("Polar", &yaw, 0.2f);

    ImGui::Separator();
    // Adapted from Online ImGui Manual
    // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
    // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
    // stored in the object itself, etc.)
    const char* items[] = {"None"
                          ,"Ruled"
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
bool UI::showCurvePanel()
{
    bool change = false;
    ImGui::Begin("Curves");

    static ImVector<ImVec2> points;
    static std::vector<glm::vec3> cps;
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;
    static bool opt_enable_context_menu = true;
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
    //static Bspline* spline = NULL;

    
    if(ImGui::Button("Save Curve"))
    {
        if(spline.controlPoints.size()>4) 
        {
            curves.push_back(spline);
            spline = Bspline();
            cps.clear();
            points.clear();

        }
        
        else ImGui::Text("No curve to save!");
    }
    
    // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
    // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
    // To use a child window instead we could use, e.g:
    //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
    //      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
    //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border, ImGuiWindowFlags_NoMove);
    //      ImGui::PopStyleColor();
    //      ImGui::PopStyleVar();
    //      [...]
    //      ImGui::EndChild();

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.x);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Add first and second point
    if (is_hovered &&  ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        //points.push_back(mouse_pos_in_canvas);
        auto normx = ((mouse_pos_in_canvas.x - 0.f)/(canvas_sz.x - 0.f) ) * (1.f - (-1.f)) + -1.f;
        auto normy = ((mouse_pos_in_canvas.y - 0.f)/(canvas_sz.x - 0.f) ) * (1.f - (-1.f)) + -1.f;
        points.push_back(ImVec2(normx,normy));
        cps.push_back(glm::vec3(normx,normy,0.f));
        std::cout<<"pushed"<<std::endl;
        //points.push_back(mouse_pos_in_canvas);
        //adding_line = true;
    }
    /*
    if (adding_line)
    {
        points.back() = mouse_pos_in_canvas;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
            adding_line = false;
    }
    */

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    /*
    const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
    }
    */

    // Context menu (under default mouse threshold)
    /*
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
    if (ImGui::BeginPopup("context"))
    {
        if (adding_line)
            points.resize(points.size() - 2);
        adding_line = false;
        if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
        if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
        ImGui::EndPopup();
    }
    */

    // Draw grid 
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 64.0f;
        for (float x = 0.f; x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
        for (float y = 0.f; y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
    }

    // Draw points + curve
    for (int n = 0; n < points.Size; n ++)
    {
        //draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
        auto scalex = ((points[n].x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
        auto scaley = ((points[n].y - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
        draw_list->AddCircle( ImVec2(origin.x + scalex, origin.y + scaley), 2.f,IM_COL32(255, 255, 0, 255) ,32.f, 0.2f);
    }
    
    if(cps.size()>4)
    {
        spline = Bspline(cps,3);
        spline.build();

        float step = 0.001;
        for (float u = 0.f; u < 1.f-step; u +=step)
        {
            //draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
            auto point1 = spline.curve(u);
            auto scalex1 = ((point1.x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            auto scaley1 = ((point1.y - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            auto point2 = spline.curve(u+step);
            auto scalex2 = ((point2.x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            auto scaley2 = ((point2.y - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            draw_list->AddLine(ImVec2(origin.x + scalex1, origin.y + scaley1), ImVec2(origin.x + scalex2, origin.y + scaley2), IM_COL32(255, 255, 0, 255), 2.0f);
        }
    }


    draw_list->PopClipRect();

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

