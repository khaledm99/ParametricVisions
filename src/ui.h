#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class UI {
    private:
        GLFWwindow* window;
        ImVec2 viewportSize;

    public:
        bool wire = false;
        int perspective = 0;
        float viewDistance = 5.f;
        int viewDirection = 0; 
        float yaw = 0.f;
        float pitch = 0.f;
        void setWindow(GLFWwindow* w);
        void initImGui();
        bool beginMainWindow();
        void endMainWindowAndRender();
        bool showConfig();
        bool showViewport(unsigned int texColBuf);
        void render();
        ImVec2 getViewportSize();
};
