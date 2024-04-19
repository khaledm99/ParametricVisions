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
    ImGui::LoadIniSettingsFromMemory(defaultLayout);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    auto verticalLine = std::vector<glm::vec3>{glm::vec3{0.f,1.f,0.f},glm::vec3{0.f,0.5f,0.f},glm::vec3{0.f,0.f,0.f},glm::vec3{0.f,-0.5f,0.f},glm::vec3{0.f,-0.75f,0.f},glm::vec3{0.f,-1.f,0.f}};
    auto shiftedVerticalLine = std::vector<glm::vec3>{glm::vec3{1.f,1.f,0.f},glm::vec3{1.f,0.5f,0.f},glm::vec3{1.f,0.f,0.f},glm::vec3{1.f,-0.5f,0.f},glm::vec3{1.f,-0.75f,0.f},glm::vec3{1.f,-1.f,0.f}};
    auto horizontalLine = std::vector<glm::vec3>{glm::vec3{-1.f,0.f,0.f},glm::vec3{-0.5f,0.f,0.f},glm::vec3{0.f,0.f,0.f},glm::vec3{0.5f,0.f,0.f},glm::vec3{1.f,0.f,0.f}};
    auto diagonalLine = std::vector<glm::vec3>{glm::vec3{0.f,1.f,0.f},glm::vec3{0.25f,0.75f,0.f},glm::vec3{0.5f,0.5f,0.f},glm::vec3{0.75f,0.25f,0.f},glm::vec3{1.f,0.f,0.f}};

    auto bigCircle = std::vector<glm::vec3>{glm::vec3{-0.0154526,0.845475,0},glm::vec3{-0.554084,0.567329,0},glm::vec3{-0.88521,-0.00220752,0},glm::vec3{-0.554084,-0.580574,0},glm::vec3{-0.0198675,-0.87638,0},glm::vec3{0.567329,-0.571744,0},glm::vec3{0.818985,0.00220752,0},glm::vec3{0.549669,0.536424,0},glm::vec3{-0.00220752,0.84106,0},};
auto littleCircle = std::vector<glm::vec3>{glm::vec3{0.540839,0.620309,0},glm::vec3{0.311258,0.611479,0},glm::vec3{0.214128,0.523179,0},glm::vec3{0.178808,0.298013,0},glm::vec3{0.192053,0.0331126,0},glm::vec3{0.284768,-0.0507727,0},glm::vec3{0.527594,-0.0860927,0},glm::vec3{0.774834,-0.0507727,0},glm::vec3{0.88521,0.0684327,0},glm::vec3{0.889625,0.275938,0},glm::vec3{0.85872,0.492274,0},glm::vec3{0.748344,0.615894,0},glm::vec3{0.584989,0.624724,0},};
    auto tinyCircle = std::vector<glm::vec3>{glm::vec3{0.554084,0.134658,0},glm::vec3{0.509934,0.108168,0},glm::vec3{0.479029,0.0860927,0},glm::vec3{0.452539,0.0375276,0},glm::vec3{0.461369,-0.0110375,0},glm::vec3{0.474614,-0.0684327,0},glm::vec3{0.501104,-0.108168,0},glm::vec3{0.536424,-0.103753,0},glm::vec3{0.571744,-0.0949228,0},glm::vec3{0.629139,-0.0728477,0},glm::vec3{0.651214,-0.0242826,0},glm::vec3{0.660044,0.0463576,0},glm::vec3{0.655629,0.0772627,0},glm::vec3{0.642384,0.103753,0},glm::vec3{0.589404,0.130243,0},glm::vec3{0.567329,0.130243,0},};
    curves.push_back(Bspline(verticalLine,3));
    curves.push_back(Bspline(shiftedVerticalLine,3));
    curves.push_back(Bspline(diagonalLine,3));
    curves.push_back(Bspline(horizontalLine,3));
    curves.push_back(Bspline(bigCircle,3));
    curves.push_back(Bspline(littleCircle,3));
    curves.push_back(Bspline(tinyCircle,3));
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

    
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            bool test;
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
    static Surface previous = NONE;
    const char* curveIndices[] = {"Vertical Line"
                                  ,"Shifted Vertical Line"
                                  ,"Diagonal Line"
                                  ,"Horizontal Line"
                                  ,"Big Circle"
                                  ,"Little Circle"
                                  ,"Tiny Circle"
                                  ,"Curve 1"
                                  ,"Curve 2"
                                  ,"Curve 3"
                                  ,"Curve 4"
                                  ,"Curve 5"
                                  ,"Curve 6"
                                  ,"Curve 7"
                                  ,"Curve 8"
                                  ,"Curve 9"
                                  ,"Curve 10"};
    switch(surfaceType) 
    {
        case RULED:
        {
            static int curve_a = 0; 
            static int curve_b = 0; 
            if(surfaceType!= previous) {
                std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
                curve_a = 0;
                curve_b = 0;
                build = true;
            }
            previous = surfaceType;
            //const char* combo_preview_value = std::to_string(curve_a).c_str();
            const char* combo_preview_value = curveIndices[curve_a];
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(curveIndices[n], is_selected))
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
            combo_preview_value = curveIndices[curve_b];
            if (ImGui::BeginCombo("Select Curve 2", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_b == n);
                        if (ImGui::Selectable(curveIndices[n], is_selected))
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
            static int p1 = 0; 
            static int q0 = 0; 
            static int q1 = 0; 
            if(surfaceType!= previous) {
                std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
                p0 = 0;
                p1 = 0;
                q0 = 0;
                q1 = 0;
                build = true;
            }
            previous = surfaceType;
            //std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
                combo_preview_value = curveIndices[p0];
                if (ImGui::BeginCombo("Select P0", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (p0 == n);
                            if (ImGui::Selectable(curveIndices[n], is_selected))
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
                combo_preview_value = curveIndices[p1];
                if (ImGui::BeginCombo("Select P1", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (p1 == n);
                            if (ImGui::Selectable(curveIndices[n], is_selected))
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
                combo_preview_value = curveIndices[q0];
                if (ImGui::BeginCombo("Select Q0", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (q0 == n);
                            if (ImGui::Selectable(curveIndices[n], is_selected))
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
                combo_preview_value = curveIndices[q1];
                if (ImGui::BeginCombo("Select Q1", combo_preview_value, 0))
                {
                    if(curves.size()>0)
                    {
                        for (int n = 0; n < curves.size(); n++)
                        {
                            const bool is_selected = (q1 == n);
                            if (ImGui::Selectable(curveIndices[n], is_selected))
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
            build |= ImGui::SliderFloat("Draw Angle", &drawAngle, 0.f,360.f);
            static int curve_a = 0; 
            static int curve_b = 0; 
            if(surfaceType!= previous) {
                std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
                curve_a = 0;
                curve_b = 0;
                build = true;
            }
            previous = surfaceType;
            //std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
            combo_preview_value = curveIndices[curve_a];
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(curveIndices[n], is_selected))
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
            combo_preview_value = curveIndices[curve_b];
            if (ImGui::BeginCombo("Select Curve 2", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_b == n);
                        if (ImGui::Selectable(curveIndices[n], is_selected))
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
            build = ImGui::SliderFloat("Draw Angle", &drawAngle, 0.f,360.f);
            static int curve_a = 0; 
            if(surfaceType!= previous) {
                std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
                curve_a = 0;
                build = true;
            }
            previous = surfaceType;
            //std::fill(returnedCurves.begin(), returnedCurves.end(), 0);
            combo_preview_value = curveIndices[curve_a];
            if (ImGui::BeginCombo("Select Curve 1", combo_preview_value, 0))
            {
                if(curves.size()>0)
                {
                    for (int n = 0; n < curves.size(); n++)
                    {
                        const bool is_selected = (curve_a == n);
                        if (ImGui::Selectable(curveIndices[n], is_selected))
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
    static bool adding_line = false;

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
    
    if(ImGui::Button("Save Curve"))
    {
        if(spline.controlPoints.size()>4) 
        {
            curves.push_back(spline);
            spline = Bspline();
            cps.clear();
            points.clear();

        }
        
    }
    if(ImGui::Button("Print Control Points"))
    {
        if(spline.controlPoints.size()>4) 
        {
            std::cout<<"std::vector<glm::vec3>{";
            for(auto p: spline.controlPoints)
            {
                std::cout<<"glm::vec3{"<<p.x<<","<<p.y<<","<<p.z<<"},";
            }
            std::cout<<"}"<<std::endl;

        }
        
    }
    
    // Canvas code from online imgui demo
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
        cps.push_back(glm::vec3(normx,-normy,0.f));
    }
    

    // Draw grid 
    auto xcol = IM_COL32(255,0,0,255);
    auto ycol = IM_COL32(0,255,0,255);
    auto grey = IM_COL32(200, 200, 200, 40);
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 32.0f;
        for (float x = 0.f; x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), ((glm::abs(canvas_sz.x/2 - x)) <8.f) ? ycol : grey);
        for (float y = 0.f; y < canvas_sz.x; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y),((glm::abs(canvas_sz.x/2 - y)) <8.f) ? xcol : grey) ;
    }

    // Draw points 
    for (int n = 0; n < points.Size; n ++)
    {
        auto scalex = ((points[n].x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
        auto scaley = ((points[n].y - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
        draw_list->AddCircle( ImVec2(origin.x + scalex, origin.y + scaley), 2.f,IM_COL32(255, 255, 0, 255) ,32.f, 0.2f);
    }
    
    // Draw curve
    if(cps.size()>4)
    {
        spline = Bspline(cps,3);
        spline.build();

        float step = 0.001;
        for (float u = 0.f; u < 1.f-step; u +=step)
        {
            auto point1 = spline.curve(u);
            auto scalex1 = ((point1.x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            auto scaley1 = ((point1.y - -1.f)/(1.f - -1.f) ) * (0 - canvas_sz.x) + canvas_sz.x;
            auto point2 = spline.curve(u+step);
            auto scalex2 = ((point2.x - -1.f)/(1.f - -1.f) ) * (canvas_sz.x - 0.f) + 0.f;
            auto scaley2 = ((point2.y - -1.f)/(1.f - -1.f) ) *(0 - canvas_sz.x) + canvas_sz.x;
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

