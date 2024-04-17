#include "ui.h"
#include "imgui_internal.h"
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
        /*
        if(firstLoop)
        {
            ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;
            ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
            ImVec2 workCenter{workPos.x + workSize.x * 0.5f, workPos.y+workSize.y*0.5f};
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGuiID root_id = ImGui::GetID("Root");
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags);
            int w, h;
            glfwGetWindowSize(window, &w, &h);
            ImVec2 size{w,h};
            ImVec2 nodePos{workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f};
            ImGui::DockBuilderSetNodeSize(dockspace_id,size);
            ImGui::DockBuilderSetNodePos(dockspace_id, nodePos);
            ImGuiID dock1 = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left,0.5f,nullptr,&dockspace_id);
            ImGuiID dock2 = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right,0.5f,nullptr,&dockspace_id);
            ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down,0.5f,nullptr,&dock2);
            ImGui::DockBuilderDockWindow("One", dock1);
            ImGui::DockBuilderDockWindow("Two", dock2);
            ImGui::DockBuilderDockWindow("Three", dock3);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        */
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
    //if(firstLoop) firstLoop = false;
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
    change |= ImGui::RadioButton("Perspective", &perspective, 0); ImGui::SameLine();
    change |= ImGui::RadioButton("Orthographic", &perspective, 1);
    ImGui::Separator();
    change |= ImGui::DragFloat("View Distance", &viewDistance, 0.1f);
    change |= ImGui::DragFloat("Azimuth", &pitch, 0.1f);
    change |= ImGui::DragFloat("Polar", &yaw, 0.1f);
    //change |= ImGui::SliderAngle("Camera Pitch", &pitch, 0, 360.f,"%.2f",0);
    //change |= ImGui::SliderAngle("Camera Yaw", &yaw, 0, 360.f,"%.2f",0);
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

