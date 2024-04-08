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

    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> controlPoints = { glm::vec3(-0.9f,-0.7f, -0.3f), glm::vec3(-0.6f,-0.4f,- 0.3f), glm::vec3(-0.5f,0.4f, -0.3f), glm::vec3(-1.0f,0.7f, -0.3f) };
    std::vector<glm::vec3> controlPoints2 = { glm::vec3(-0.7f,-0.7f, 0.3f), glm::vec3(-0.4f,-0.4f, 0.3f), glm::vec3(-0.3f,0.4f, 0.3f), glm::vec3(-0.7f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints3 = { glm::vec3(-0.3,-0.7f, 0.3f), glm::vec3(-0.0f,-0.4f, 0.3f), glm::vec3(0.1f,0.4f, 0.3f), glm::vec3(-0.3f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints4 = { glm::vec3(-0.1,-0.7f, 0.3f), glm::vec3(0.2f,-0.4f, 4.3f), glm::vec3(0.3f,0.4f, 0.3f), glm::vec3(-0.1f,0.7f, 0.3f) };
    std::vector<glm::vec3> controlPoints5 = { glm::vec3(0.4,-1.7f, 0.7f), glm::vec3(0.6f,-1.7f, 0.7f), glm::vec3(0.5f,0.4f, 0.7f), glm::vec3(1.1f,0.7f, 0.7f) };

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




    Shader s("./shaders/shader.vs", "./shaders/shader.fs"); 
    Shader axis("./shaders/axis.vs", "./shaders/axis.fs"); 
   
    Framebuffer fb;
    fb.setup(VPWIDTH, VPHEIGHT);

    auto vs = ui.getViewportSize();
    vs.x = VPWIDTH;
    vs.y = VPHEIGHT;
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

        Camera cam;
        
       
        switch(ui.viewDirection)
        {
            case(0):
                cam.update(glm::vec3(-10.f,0.f,0.f), 0.f,0.f, -10.f);
                break;
            case(1):
                cam.update(glm::vec3(0.f,0.f,-10.f), 90.f,0.f, -10.f);
                break;
            case(2):
                cam.update(glm::vec3(0.f,10.f,0.f), 0.f,-90.f, -10.f);
                break;
            case(3):
                cam.update(glm::vec3(-5.f,5.f,5.f), -45.f,-35.f, -10.f);
            case(4):
                break;

        }

        glm::mat4 view = cam.lookAt();
        glm::mat4 model = glm::mat4(1.f);

        s.setVec3("viewPos",cam.pos);

        glm::mat4 projection;
        float aspect = vs.x/vs.y;
        if(ui.perspective == 0)
            projection = glm::perspective(glm::radians(45.f),aspect,0.1f,100.f);
        else projection = glm::ortho(-aspect, aspect, -1.f, 1.f, 0.1f, 100.0f);
        
        // Send matrices to shader
        s.setMat4("model", model);
        s.setMat4("projection", projection);
        s.setMat4("view", view);
        // draw cube
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //rs.draw();
        ffSurface.draw();


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

        bspline.draw();

        bspline2.draw();


        

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
