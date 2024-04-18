#include "ui.h"
#include "shader.h"
#include "framebuffer.h"
#include "Line.h"
#include "Triangle.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "RevolutionSurface.h"
#include "Bspline.h"
#include "FreeformSurface.h"
#include "RotBlendSurface.h"
#include "ruledsurface.h"
#include "bilinearpatch.h"
#include "coonspatch.h"

#include <stdio.h>
#include <iostream>



bool resized = false;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    resized = true;
}
bool wire = false;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        wire = !wire;

    }

}



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int WWIDTH = 1920;
    const int WHEIGHT = 1080;
    const int VPWIDTH = 800;
    const int VPHEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(WWIDTH,WHEIGHT, "ParametricVisions", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0,0,VPWIDTH,VPHEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
	


    UI ui;
    ui.setWindow(window);
    ui.initImGui();

      
    glEnable(GL_DEPTH_TEST);


    Shader s("./shaders/shader.vs", "./shaders/shader.fs"); 
    Shader axis("./shaders/axis.vs", "./shaders/axis.fs"); 
   
    Framebuffer fb;
    fb.setup(VPWIDTH, VPHEIGHT);

    auto vs = ui.getViewportSize();
    vs.x = VPWIDTH;
    vs.y = VPHEIGHT;
    Camera cam;


    Surface selected = NONE;
    RuledSurface ruled;
    CoonsPatch coons;
    RevolutionSurface revolution;
    RotationalBlendSurface rotational;
    while(!glfwWindowShouldClose(window))
    {
        
        glfwPollEvents();
        processInput(window);

        // Set wireframe mode
        fb.bind();
        if(ui.wire)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        s.use();

        cam.update(glm::vec3(-1.f,0.f,0.f), ui.yaw,ui.pitch, ui.viewDistance);
        glm::mat4 view = cam.lookAt();

        glm::mat4 model = glm::mat4(1.f);
        s.setVec3("viewPos",cam.pos);

        glm::mat4 projection;
        float aspect = vs.x/vs.y;
        if(ui.perspective == 0)
        {
            projection = glm::perspective(glm::radians(45.f),aspect,0.1f,100.f);
            model = glm::mat4(1.f);
        }
        else 
        {
            projection = glm::ortho(-aspect, aspect, -1.f, 1.f, -100.f, 100.f);
            model = glm::scale(glm::mat4(1.f), glm::vec3(0.2f));
        }
        
        // Send matrices to shader
        s.setMat4("model", model);
        s.setMat4("projection", projection);
        s.setMat4("view", view);

        switch(ui.surfaceType)
        {
            case RULED:
                {
                    if(ui.returnedCurves.size()>0)
                    {
                        if(ui.build)
                        {
                            ruled.curve_a = ui.curves[ui.returnedCurves[0]];
                            ruled.curve_b = ui.curves[ui.returnedCurves[1]];
                            ruled.build();
                            ui.build = false;
                        }
                    }
                    if(ui.showSurface) ruled.draw();
                    if(ui.showCurves) ruled.drawCurves();
                }
                break;
            case COONS:
                {
                    if(ui.returnedCurves.size()>0)
                    {
                        if(ui.build)
                        {
                            coons.step = ui.coonsStep;
                            coons.p0 = ui.curves[ui.returnedCurves[0]];
                            coons.p1 = ui.curves[ui.returnedCurves[1]];
                            coons.q0 = ui.curves[ui.returnedCurves[2]];
                            coons.q1 = ui.curves[ui.returnedCurves[3]];
                            coons.build();
                            ui.build = false;
                        }
                    }
                    if(ui.showSurface) coons.draw();
                    if(ui.showCurves) coons.drawCurves();

                }
                break;
            case REVOLUTION:
                {
                    if(ui.returnedCurves.size()>0)
                    {
                        if(ui.build)
                        {
                            revolution.curve = ui.curves[ui.returnedCurves[0]];
                            revolution.build();
                            ui.build = false;
                        }
                    }
                    if(ui.showSurface) revolution.draw();
                    if(ui.showCurves) revolution.drawCurves();

                }
                break;
            case ROTATIONAL:
                {
                    if(ui.returnedCurves.size()>0)
                    {
                        if(ui.build)
                        {
                            rotational.curve1 = ui.curves[ui.returnedCurves[0]];
                            rotational.curve2 = ui.curves[ui.returnedCurves[1]];
                            rotational.build();
                            ui.build = false;
                        }
                    }
                    if(ui.showSurface) rotational.draw();
                    if(ui.showCurves) rotational.drawCurves();

                }
                break;
         
        }

        
        glBindVertexArray(0);

        // Draw axes
        axis.use();
        s.setMat4("model", model);
        s.setMat4("projection", projection);
        s.setMat4("view", view);


        glm::vec3 RED(1.f,0.f,0.f);
        glm::vec3 GREEN(0.f,1.f,0.f);
        glm::vec3 YELLOW(1.f,1.f,0.f);
        Line xaxis(glm::vec3(100.f,0.f,0.f),glm::vec3(-100.f,0.f,0.f),RED);
        xaxis.draw();
        Line yaxis(glm::vec3(0.f,100.f,0.f),glm::vec3(0.f,-100.f,0.f),YELLOW);
        yaxis.draw();
        Line zaxis(glm::vec3(0.f,0.f,100.f),glm::vec3(0.f,0.f,-100.f),GREEN);
        zaxis.draw();


        //rbs.drawMidline();


        glBindVertexArray(0);
        fb.unbind();


        // UI 
        bool change = false;

        change |= ui.beginMainWindow();
        change |= ui.showConfig();
        change |= ui.showViewport(fb.getTextureBuffer());
        change |= ui.showCurvePanel();
        ui.endMainWindowAndRender();
        
        ui.render();

        // Resize viewport framebuffer if ui panel is resized
        if(vs.x != ui.getViewportSize().x || vs.y != ui.getViewportSize().y || resized)
        {
            vs = ui.getViewportSize();
            fb.setup(vs.x, vs.y);
            resized = false;
        }
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
