#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Bspline.h"
#include <vector>

enum Surface {NONE = 0
             ,RULED
             ,COONS
             ,ROTATIONAL
             ,REVOLUTION
};

class UI {
    private:
        GLFWwindow* window;
        ImVec2 viewportSize;
        const char* defaultLayout = "[Window][DockSpace Demo]\nSize=1920,1080\nCollapsed=0\n[Window][Debug##Default]\nPos=60,60\nSize=400,400\nCollapsed=0\n[Window][Config]\nPos=0,19\nSize=423,1061\nCollapsed=0\nDockId=0x00000001,0\n[Window][Viewport]\nPos=425,19\nSize=1024,1061\nCollapsed=0\nDockId=0x00000002,0\n[Window][Curves]\nPos=1451,19\nSize=469,1061\nCollapsed=0\nDockId=0x00000004,0\n[Docking][Data]\mDockSpace     ID=0x3BC79352 Window=0x4647B76E Pos=0,19 Size=1920,1061 Split=X Selected=0x13926F0B\nDockNode    ID=0x00000001 Parent=0x3BC79352 SizeRef=1449,1061 Split=X\nDockNode  ID=0x00000002 Parent=0x00000001 SizeRef=422,1061 Selected=0x94B17C7D\nDockNode  ID=0x00000003 Parent=0x00000001 SizeRef=1496,1061 CentralNode=1 Selected=0x13926F0B\nDockNode    ID=0x00000005 Parent=0x3BC79352 SizeRef=469,1061 Selected=0x0337FD01";

    public:

        Bspline spline;
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
        bool showCurvePanel();
        bool showViewport(unsigned int texColBuf);
        void render();
        ImVec2 getViewportSize();
};
