#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class UI {
    private:
        GLFWwindow* window;

    public:
        bool wire = false;
        void setWindow(GLFWwindow* w);
        void initImGui();
        bool beginMainWindow();
        void endMainWindowAndRender();
        bool showConfig();
        void render();
};
