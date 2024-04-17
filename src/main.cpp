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



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
// from https://nerdhut.de/2019/12/04/arcball-camera-opengl/
void mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    // Whenever the left mouse button is pressed, the
    // mouse cursor's position is stored for the arc-
    // ball camera as a reference.
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double curr_x = 0, curr_y = 0;
 
        glfwGetCursorPos(window, &curr_x, &curr_y);
 
        // last is a global vec3 variable
        //last = vec3(curr_x, curr_y, -1);
 
        // This is another global variable
        //ballEnabled = true;
    }
 
    // When the user releases the left mouse button,
    // all we have to do is to reset the flag.
    //if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        //ballEnabled = false;
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
	
    glfwSetMouseButtonCallback(window, mouseCallback);


    UI ui;
    ui.setWindow(window);
    ui.initImGui();
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.f,1.f,0.f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.f,1.f,0.f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.f,1.f,0.f, 0.0f,  1.0f,  0.0f
};
      

/*
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    */

    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> controlPoints = { glm::vec3(-0.9f,-0.7f, -0.3f), glm::vec3(-0.6f,-0.4f,- 0.3f), glm::vec3(-0.5f,0.4f, -0.3f), glm::vec3(-1.0f,0.7f, -0.3f) };
    std::vector<glm::vec3> controlPoints2 = { glm::vec3(-0.7f,-0.7f, 0.3f), glm::vec3(-0.4f,-0.4f, 0.3f), glm::vec3(-0.3f,0.4f, 0.3f), glm::vec3(-0.7f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints3 = { glm::vec3(-0.3,-0.7f, 0.3f), glm::vec3(-0.0f,-0.4f, 0.3f), glm::vec3(0.1f,0.4f, 0.3f), glm::vec3(-0.3f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints4 = { glm::vec3(-0.1,-0.7f, 0.3f), glm::vec3(0.2f,-0.4f, 4.3f), glm::vec3(0.3f,0.4f, 0.3f), glm::vec3(-0.1f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints5 = { glm::vec3(0.4,-1.7f, 0.7f), glm::vec3(0.6f,-1.7f, 0.7f), glm::vec3(0.5f,0.4f, 0.7f), glm::vec3(1.1f,0.7f, 0.7f) };


    std::vector<glm::vec3> controlPoints6 = { glm::vec3(0.4f, 0.8f, -1.f), glm::vec3(0.7f, 0.4f, -1.f), glm::vec3(0.4f, 0.f, -1.f) };

    std::vector<glm::vec3> controlPoints7 = { glm::vec3(0.1f, 0.8f, -1.f), glm::vec3(0.1f, 0.4f, -1.f), glm::vec3(0.1f, 0.f, -1.f) };

    std::vector<glm::vec3> controlPoints8 = { glm::vec3(-5.f, 0.8f, -3.f),glm::vec3(-5.f, -2.f, 1.f), glm::vec3(-5.f, 1.5f, -2.f), glm::vec3(-5.f, 0.2f, 2.f) };
    std::vector<glm::vec3> controlPoints9 = { glm::vec3(5.f, 0.8f, -3.f), glm::vec3(5.f, 1.5f, -2.f), glm::vec3(5.f, 0.2f, 2.f) };
    std::vector<glm::vec3> controlPoints11 = { glm::vec3(-5.f, 0.8f, -3.f), glm::vec3(1.f, 3.f, -3.f), glm::vec3(5.f, 0.8f, -3.f) };
    std::vector<glm::vec3> controlPoints12 = { glm::vec3(-5.f, 0.2f, 2.f), glm::vec3(1.f, -2.f, 3.f), glm::vec3(5.f, 0.2f, 2.f) };

    std::vector<glm::vec3> P0cps = {glm::vec3(-1.f,-1.f,0.f),glm::vec3(0.f,1.f,0.f),glm::vec3(1.f,0.f,0.f)};
    std::vector<glm::vec3> P1cps = {glm::vec3(-1.f,0.f,0.f),glm::vec3(0.f,-1.f,0.f),glm::vec3(1.f,1.f,0.f)};
    std::vector<glm::vec3> Q0cps = {glm::vec3(-1.f,-1.f,0.f),glm::vec3(-0.3f,1.f,0.f),glm::vec3(1.f,0.f,0.f)};
    std::vector<glm::vec3> Q1cps = {glm::vec3(-1.f,0.f,0.f),glm::vec3(0.3f,1.f,0.f),glm::vec3(1.f,1.f,0.f)};
    Bspline p0(P0cps,3);
    p0.build();
    Bspline p1(P1cps,3);
    p1.build();
    Bspline q0(Q0cps,3);
    q0.build();
    Bspline q1(Q1cps,3);
    q1.build();


    Bspline rbsc1(controlPoints6, 3);
    rbsc1.build();
    Bspline rbsc2(controlPoints7, 3);
    rbsc2.build();

    Bspline rlcp1(controlPoints8, 3);
    rlcp1.build();
    Bspline rlcp2(controlPoints9, 3);
    rlcp2.build();
    Bspline rlcp3(controlPoints11, 3);
    rlcp3.build();
    Bspline rlcp4(controlPoints12, 3);
    rlcp4.build();
    RuledSurface s0(rlcp1,rlcp2);
    s0.build();
    RuledSurface s1(rlcp3,rlcp4);
    s1.build();
    BilinearPatch s2(controlPoints8[3],controlPoints8[0],controlPoints12[2],controlPoints9[0]);
    s2.build();
    CoonsPatch c(p0,p1,q0,q1);
    c.build();



    std::vector<glm::vec3> controlPoints10 = { glm::vec3(5.f, 2.f, 5.f), glm::vec3(5.f, 0.5f, -5.f), glm::vec3(-5.f, 0.2f, 5.f),glm::vec3(-5.f, 1.f, -5.f) };

    std::vector<std::vector<glm::vec3>> freeformPoints;
    freeformPoints.push_back(controlPoints);
    freeformPoints.push_back(controlPoints2);
    freeformPoints.push_back(controlPoints3);
    freeformPoints.push_back(controlPoints4);
    freeformPoints.push_back(controlPoints5);


    FreeformSurface ffSurface(freeformPoints, 3, 3);
    ffSurface.build();

    Bspline bspline(controlPoints, 3);
    bspline.build();

    Bspline bspline2(controlPoints5, 3);
    bspline2.build();

    RevolutionSurface rs(bspline);
    rs.build();

    RotationalBlendSurface rbs(rbsc1, rbsc2);
    rbs.build();

    //RuledSurface ruled(rlsc1,rlsc2);
    //ruled.build();

    BilinearPatch bp(controlPoints10[0],controlPoints10[1],controlPoints10[2],controlPoints10[3]);
    bp.build();


    Shader s("./shaders/shader.vs", "./shaders/shader.fs"); 
    Shader axis("./shaders/axis.vs", "./shaders/axis.fs"); 
   
    Framebuffer fb;
    fb.setup(VPWIDTH, VPHEIGHT);

    auto vs = ui.getViewportSize();
    vs.x = VPWIDTH;
    vs.y = VPHEIGHT;
    Camera cam;
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

        // draw cube
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //rs.draw();
        //ffSurface.draw();
        //rbs.draw();
        //ruled.draw();

        //bp.draw();
        c.draw();

        //rlcp1.draw();
        //rlcp2.draw();
        //rlcp3.draw();
        //rlcp4.draw();
        //s0.draw();
        //RuledSurface s0(rlcp1,rlcp2);
        //RuledSurface s1(rlcp3,rlcp4);

        //p0.draw();
        //p1.draw();
        //q0.draw();
        //q1.draw();


        /*
        std::vector<float> sphereverts;
        float R = 2.f;
        float step = 0.01;
        for(float u=0.f; u<3.14f; u+=step)
        {
            for(float v=0.f; v<3.14f*2.f; v+=step)
            {
                sphereverts.push_back(R*glm::sin(u)*glm::cos(v));
                sphereverts.push_back(R*glm::sin(v)*glm::sin(u));
                sphereverts.push_back(R*glm::cos(u));
                sphereverts.push_back(0.f);
                sphereverts.push_back(0.f);
                sphereverts.push_back(1.f);
                sphereverts.push_back(-glm::sin(v)*glm::cos(u));
                sphereverts.push_back(-glm::sin(v)*glm::sin(u));
                sphereverts.push_back(-glm::cos(v));
            }
            
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphereverts.size(), sphereverts.data(), GL_STATIC_DRAW);
        glPointSize(10.f);
        glDrawArrays(GL_POINTS, 0, sphereverts.size()/3);
        */
        
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
        ui.endMainWindowAndRender();
        
        ui.render();

        // Resize viewport framebuffer if ui panel is resized
        if(vs.x != ui.getViewportSize().x || vs.y != ui.getViewportSize().y)
        {
            vs = ui.getViewportSize();
            fb.setup(vs.x, vs.y);
        }
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
