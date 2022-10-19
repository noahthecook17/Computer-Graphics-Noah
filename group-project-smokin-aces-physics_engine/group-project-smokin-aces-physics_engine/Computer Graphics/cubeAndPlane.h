#pragma once
#ifndef MOVING_CUBE
#define MOVING_CUBE
//glad is used for retriveing opengl functions? 
#include <glad/glad.h>

//graphics language framework
#include <GLFW/glfw3.h>

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  
#include <chrono>

#include <iostream>

//custom shader program
#include "Shader_H.h"

//glm mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "learnopengl_camera.h"

#include "modelLoader.h"
#include "BoundingBox.h"
//#include "phsyics.h"

Camera camera = Camera();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((float)(2 * yoffset));
}

bool firstTime = true;
double lastXpos = 0.0f, lastYpos = 0.0f;
float lastFrame = 0.0f;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstTime) {
        lastXpos = xpos;
        lastYpos = ypos;
        firstTime = false;
        camera.MouseSensitivity = 0.08f;
    }
    camera.ProcessMouseMovement(xpos - lastXpos,  lastYpos - ypos);

    lastXpos = xpos;
    lastYpos = ypos;
}


void processInput(GLFWwindow* window)
{
    //state == 0 is stationairy
    //state == 1 is falling
    //state == 2 is jumping
    float deltaTime = 0.0f;
   
    float currentFrame = glfwGetTime();

    deltaTime = currentFrame - lastFrame;

    lastFrame = currentFrame;

    float speed = 4.0f * deltaTime;
    


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
   

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, speed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, speed);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, speed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, speed);
    }    

    glfwSetScrollCallback(window, scroll_callback);
    

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {        
        camera.Zoom = 45.0f;
    }



};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
};

class CubeAndPlane{   

    
public:
    static int run() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

        if (window == NULL)
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
        };

        unsigned int texture;
        unsigned int background;
        unsigned int fish_scales;
        glGenTextures(1, &texture);
        glGenTextures(1, &background);
        glGenTextures(1, &fish_scales);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load("container.png", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, background);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("woodfloor.png", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, fish_scales);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("fish_scales.jpg", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        unsigned int indices[] = {
            0, 3, 1,
            1, 2, 3
        };

        //bind VAO and configure VBO and attribarray
        unsigned int VAO1;
        glGenVertexArrays(1, &VAO1);

        glBindVertexArray(VAO1);

        unsigned int VBO1;
        glGenBuffers(1, &VBO1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //points to data in the vertex buffer object for use in vertex shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(3);

        unsigned int lightVAO;
        glGenVertexArrays(1, &lightVAO);

        glBindVertexArray(lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //this is used for input to the vertex shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //Element Buffer Object
        //unsigned int EBO;
        //glGenBuffers(1, &EBO);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //uses uniforms to do transformations in the vertex shader and color in the fragment shader
        Shader ourShader("vertex.glsl", "fragment.glsl");

        Model ourModel("fish_1.obj");
        
        BoundingBox * fish1 = ourModel.getBoundingBox(true);

        BoundingBox* fish2 = ourModel.getBoundingBox(true);

        //BoundingBox* fish3 = ourModel.getBoundingBox(true);

        
        fish1->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, -50.0f)), false);

        fish2->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 50.0f, -50.0f)), false);

        ///fish3->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 1.0f, -10.0f)), false);

        fish1->increaseAcceleration(glm::vec3(-1.0f, 0.0f, 1.5f));

        fish2->increaseAcceleration(glm::vec3(1.0f, 0.0f, 1.5f));

        //fish3->increaseAcceleration(glm::vec2(0.2f, 0.2f));
        
       //import diver

        Model diverModel("BetterDiver.obj");

        BoundingBox* diver = diverModel.getBoundingBox(true);

        diver->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 10.0f, 25.0f)), false);
        /*
        std::cout << model0->getMax().x;
        std::cout << model0->getMin().x;

        std::cout << model0->getMax().y;
        std::cout << model0->getMin().y;

        std::cout << model0->getMax().z;
        std::cout << model0->getMin().z;
        */

        glViewport(0, 0, 800, 600);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glm::vec3 lightLocation(0.0f, 1.0f, -1.0f);
               
        
        int size = sizeof(vertices)/sizeof(vertices[0]);
        float floorVertices[] = {
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
        };
        int sizeOfFloor = sizeof(floorVertices) / sizeof(floorVertices[0]);
        BoundingBox *model1 = new BoundingBox(vertices, size, true);
        //BoundingBox *model2 = new BoundingBox(vertices, size, true);
        BoundingBox *model3 = new BoundingBox(floorVertices, sizeOfFloor, false);
        BoundingBox *model4 = new BoundingBox(vertices, size, true);

        model1->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 10.0f, 0.0f)), false);

        glm::mat4 modelt = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        modelt = glm::scale(modelt, glm::vec3(100.0f, 1.0f, 100.0f));
        
        model3->transformModel(modelt, false);
        
        /*
        Model *models[] = {
            model1,
            model2,
            model3,
            model4
        };     */
       
        model4->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 20.0f, 0.0f)), false);        
        //model2->transformModel(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)), false);
        
        glm::mat4 fishTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

        //model4->increaseAcceleration(glm::vec2(0.0f, -3.0f));
        //model1->increaseAcceleration(glm::vec2(1.0f, 1.0f));
        
        camera.Position = glm::vec3(model1->getCenter().x, model1->getCenter().y + 2.0f, camera.Position.z + 10.0f);

        bool devcamera = false;

        while (!glfwWindowShouldClose(window)) {  

            if (!devcamera) {
                camera.Position = glm::vec3(model1->getCenter().x, model1->getCenter().y + 2.0f, camera.Position.z);
            }
            else {
                camera.Position = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z);
            }

            if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
                devcamera = true;
            }
            if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
                devcamera = false;
            }

            glfwSetCursorPosCallback(window, cursor_position_callback);            

            //clear the color from the previous frame
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            BoundingBox::updateLocations();

            //input
            //processInput(window, &vertical, &horizontal, &spinx, &spiny, &spinz, characterState);

            //view and projection transforms
            glm::mat4 view = camera.GetViewMatrix();
            processInput(window);            
                                  
            
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
            
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                model1->movement(glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                model1->movement(glm::vec3(-1.0f, 0.0f, 0.0f), 10.0f);
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                model1->movement(glm::vec3(0.0f, 1.0f, 0.0f), 200.0f);
            }
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                model1->reset();
            }
            /*
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                model1->transformModel( glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.001f, 0.0f)), true);
            } 
            */
            

            //rendering commands      
            glBindVertexArray(VAO1);
            ourShader.use();

            //uniforms for shader 1
            GLuint modelUniform = glGetUniformLocation(ourShader.ID, "model");
            GLuint viewUniform = glGetUniformLocation(ourShader.ID, "view");
            GLuint projectionUniform = glGetUniformLocation(ourShader.ID, "projection");            

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));


            //model1.transformModel(model);

            //std::cout << model1.doesCollide(model2);
            //model1.doesCollide(model2);
            
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model1->getTransform()));
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
            
            GLuint colorUniform = glGetUniformLocation(ourShader.ID, "ourColor");

            //setup light
            glm::vec3 lightPosition = glm::vec3(3.0f, 3.0f, 0.0f);
            glm::vec3 lightAmbient = glm::vec3(0.05f);
            glm::vec3 lightDiffuse = glm::vec3(1.0f);
            glm::vec3 lightSpecular = glm::vec3(1.0f);

            ourShader.setVec3("viewPos", camera.Position);

            ourShader.setVec3("lightPos", glm::vec3(lightPosition));
            ourShader.setVec3("light.ambient", glm::vec3(lightAmbient));
            ourShader.setVec3("light.diffuse", glm::vec3(lightDiffuse));
            ourShader.setVec3("light.specular", glm::vec3(lightSpecular));
            ourShader.setFloat("light.quadratic", 0.0007f);
            ourShader.setFloat("light.linear", 0.14f);
            ourShader.setFloat("light.constant", 1.0f);            

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model1->getTransform()));
            glBindTexture(GL_TEXTURE_2D, fish_scales);
            //ourModel.Draw(ourShader);

            model = glm::mat4(1.0f);
            
            glBindVertexArray(VAO1);
            ourShader.use();

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model1->getTransform()));

            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model2->getTransform()));

            //glBindTexture(GL_TEXTURE_2D, texture);
           // glDrawArrays(GL_TRIANGLES, 0, 36);

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model4->getTransform()));

            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
           

            glBindTexture(GL_TEXTURE_2D, fish_scales);

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(fish1->getTransform()));
            ourModel.Draw(ourShader);

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(fish2->getTransform()));
            ourModel.Draw(ourShader);

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(diver->getTransform()));
            diverModel.Draw(ourShader);
            //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(fish3->getTransform()));
            //ourModel.Draw(ourShader);

            /*
            for (int i = 0; i < 10; i++) {
                
                fishTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-(glfwGetTime()  + 4*i)+ 100.0f, 1.0f, -(-10 + 4* i)));
                fishTransform = glm::rotate(fishTransform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(fishTransform));
                ourModel.Draw(ourShader);
            }
            */
            
            

            

            

            
            

            //Background
            //model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            

           glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model3->getTransform()));

            glBindVertexArray(VAO1);

            glBindTexture(GL_TEXTURE_2D, background);
            glDrawArrays(GL_TRIANGLES, 30, 6);

            //glDrawArrays(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
           // glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model0->getTransform()));
            //glBindTexture(GL_TEXTURE_2D, fish_scales);
            //ourModel.Draw(ourShader);            

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));


            glBindVertexArray(lightVAO);
            glBindTexture(GL_TEXTURE_2D, background);
            model = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition));
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniform4f(colorUniform, 1.0f, 1.0f, 1.0f, 1.0f);
            glDepthMask(GL_FALSE);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glDepthMask(GL_TRUE);
            glUniform4f(colorUniform, 0.0f, 1.0f, 1.0f, 1.0f);
            //check call events and swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
};
#endif