#include "ui.h"
#include "shader.h"
#include "framebuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"

#include <stdio.h>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}
bool wire = false;
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
        std::cout<< "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0,0,VPWIDTH,VPHEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    UI ui;
    ui.setWindow(window);
    ui.initImGui();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
        

    unsigned int VBO;
    glGenBuffers(1,&VBO);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int lVBO;
    glGenBuffers(1,&lVBO);
    unsigned int lVAO;
    glGenVertexArrays(1, &lVAO);

    glBindVertexArray(lVAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(lineverts), lineverts, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    Shader s("./shaders/shader.vs", "./shaders/shader.fs"); 
   
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
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        s.use();

        // Setup camera matrices
        glm::mat4 model = glm::mat4(1.f);
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f,1.f,0.f));
        glm::mat4 initview = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.f,-ui.viewDistance));
        glm::mat4 view;
        switch(ui.viewDirection)
        {
            case(0):
                view = initview;
                break;
            case(1):
                view = glm::rotate(initview,glm::radians(90.f) , glm::vec3(0.f,1.f,0.f));
                break;
            case(2):
                view = glm::rotate(initview,glm::radians(90.f) , glm::vec3(1.f,0.f,0.f));
                break;
            case(3):
                view = glm::rotate(initview,glm::radians(35.f) , glm::vec3(1.f,0.f,0.f));
                view = glm::rotate(view,glm::radians(-45.f) , glm::vec3(0.f,1.f,0.f));
                break;
        }
        glm::mat4 projection;
        float aspect = vs.x/vs.y;
        if(ui.perspective == 0)
            projection = glm::perspective(glm::radians(45.f),aspect,0.1f,100.f);
        else projection = glm::ortho(-aspect, aspect, -1.f, 1.f, 0.1f, 100.0f);
        
        glm::vec3 color(0.f,10.f,34.f);
        // Send matrices to shader
        int modelLoc = glGetUniformLocation(s.id,"model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        modelLoc = glGetUniformLocation(s.id,"view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));
        modelLoc = glGetUniformLocation(s.id,"projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));
        modelLoc = glGetUniformLocation(s.id,"col");
        glUniform3f(modelLoc, color.x,color.y,color.z);

        // draw cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw axes
        model = glm::mat4(1.f);
        modelLoc = glGetUniformLocation(s.id,"model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(lVAO);
        float lineverts[] = {
            0.f, -100.f, 0.f,
            0.f, 100.f ,0.f
        };
        color = glm::vec3(0.f,1.f,0.f);
        modelLoc = glGetUniformLocation(s.id,"col");
        glUniform3f(modelLoc, color.x,color.y,color.z);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineverts), lineverts, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);
        lineverts[0] = -100.f;
        lineverts[1] = 0.f;
        lineverts[3] = 100.f;
        lineverts[4] = 0.f;
        color = glm::vec3(1.f,0.f,0.f);
        modelLoc = glGetUniformLocation(s.id,"col");
        glUniform3f(modelLoc, color.x,color.y,color.z);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineverts), lineverts, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);
        lineverts[0] = 0.f;
        lineverts[2] = -100.f;
        lineverts[3] = 0.f;
        lineverts[5] = 100.f;
        color = glm::vec3(0.f,0.f,1.f);
        modelLoc = glGetUniformLocation(s.id,"col");
        glUniform3f(modelLoc, color.x,color.y,color.z);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineverts), lineverts, GL_STATIC_DRAW);
        glDrawArrays(GL_LINES, 0, 2);

        std::vector<float> sphereverts;
        float R = 2.f;
        float step = 0.05;
        for(float u=0.f; u<3.14f; u+=step)
        {
            for(float v=0.f; v<3.14f*2.f; v+=step)
            {
                sphereverts.push_back(R*glm::sin(u)*glm::cos(v));
                sphereverts.push_back(R*glm::sin(v)*glm::sin(u));
                sphereverts.push_back(R*glm::cos(u));
            }
            
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphereverts.size(), sphereverts.data(), GL_STATIC_DRAW);
        glPointSize(10.f);
        glDrawArrays(GL_POINTS, 0, sphereverts.size()/3);


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
