#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Bspline.h"
#include <vector>

enum Surface {NONE = 0
             ,RULED
             ,BILINEAR
             ,COONS
             ,ROTATIONAL
             ,REVOLUTION
};

class UI {
    private:
        GLFWwindow* window;
        ImVec2 viewportSize;

    public:

        std::vector<Bspline> curves;
        std::vector<int> returnedCurves;
        bool wire = false;
        bool showSurface = true;
        bool showCurves = true;
        bool build = false;
        int perspective = 0;
        float viewDistance = -5.f;
        float yaw = 0.f;
        float pitch = 0.f;
        Surface surfaceType = NONE;
        int coonsStep = 4;
        void setWindow(GLFWwindow* w);
        void initImGui();
        bool beginMainWindow();
        void endMainWindowAndRender();
        bool showConfig();
        bool showViewport(unsigned int texColBuf);
        void render();
        ImVec2 getViewportSize();
};
