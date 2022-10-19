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
#include <vector>

#include <iostream>

//custom shader program
#include "Shader_H.h"

//glm mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"

#include "learnopengl_camera.h"

#include "modelLoader.h"
#include "BoundingBox.h"
#include "PhysicsModel.h"
//#include "phsyics.h"
#include "input.h"
#include "Scene.h"

bool firstTime = true;
double lastXpos = 0.0f, lastYpos = 0.0f;
float lastFrame = 0.0f;
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstTime) {
        lastXpos = xpos;
        lastYpos = ypos;
        firstTime = false;
        //camera.MouseSensitivity = 0.08f;
    }
    //camera.ProcessMouseMovement(xpos - lastXpos,  lastYpos - ypos);

    lastXpos = xpos;
    lastYpos = ypos;

    //playerModel->rotate(lastYpos - ypos);
}

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
        unsigned int grass;
        unsigned int blending_window;

        glGenTextures(1, &blending_window);
        glGenTextures(1, &texture);
        glGenTextures(1, &background);
        glGenTextures(1, &fish_scales);
        glGenTextures(1, &grass);

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

        data = stbi_load("rock.jpg", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        std::cout << glm::to_string(glm::translate(glm::identity<glm::mat4>(), glm::vec3(1.0f, 1.0f, 1.0f))) << "\n";

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

        glBindTexture(GL_TEXTURE_2D, grass);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("grass.png", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);

        std::vector<string> textures_faces = {
            "skybox\\right.jpg",
            "skybox\\left.jpg",
            "skybox\\top.jpg",
            "skybox\\bottom.jpg",
            "skybox\\front.jpg",
            "skybox\\back.jpg"
        };

        unsigned int cubemap;

        glGenTextures(1, &cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        for (unsigned int i = 0; i < textures_faces.size(); i++)
        {

            data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else {
                std::cout << "\n" << "failed to load texture" << "\n";
                stbi_image_free(data);
            }            
        }

        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        unsigned int skyvao;
        glGenVertexArrays(1, &skyvao);
        glBindVertexArray(skyvao);

        unsigned int skyvbo;
        glGenBuffers(1, &skyvbo);
        glBindBuffer(GL_ARRAY_BUFFER, skyvbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        Shader cubeShader("cubevertex.glsl", "cubefragment.glsl", nullptr);        

        glBindTexture(GL_TEXTURE_2D, blending_window);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("blending_transparent_window.png", &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

        float quad[] = {
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f
        };

        unsigned int QUADVAO;
        glGenVertexArrays(1, &QUADVAO);

        glBindVertexArray(QUADVAO);

        unsigned int QUADVBO;
        glGenBuffers(1, &QUADVBO);

        glBindBuffer(GL_ARRAY_BUFFER, QUADVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        Shader quadShader("vertexBuffer.glsl", "fragmentBuffer.glsl", nullptr);

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

        unsigned int vegetationVao;
        glGenVertexArrays(1, &vegetationVao);

        glBindVertexArray(vegetationVao);

        unsigned int VBO2;
        glGenBuffers(1, &VBO2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //this is used for input to the vertex shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        //Element Buffer Object
        //unsigned int EBO;
        //glGenBuffers(1, &EBO);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //uses uniforms to do transformations in the vertex shader and color in the fragment shader
        Shader ourShader("vertex.glsl", "fragment.glsl", nullptr);
        Shader outlineShader("vertexOutline.glsl", "fragOutline.glsl", nullptr);
        Shader geometryTester("vertexg.glsl", "fragmentg.glsl", "geometry.glsl");       

        
        Model* ourModel = new Model("fish_1.obj");

        glViewport(0, 0, 800, 600);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glDepthFunc(GL_LEQUAL);        

        glEnable(GL_PROGRAM_POINT_SIZE);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glm::vec3 lightLocation(0.0f, 1.0f, -1.0f);            
        
        
        
        /*
        Model *models[] = {
            model1,
            model2,
            model3,
            model4
        };     */

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        unsigned int frameTexture;
        glGenTextures(1, &frameTexture);
        glBindTexture(GL_TEXTURE_2D, frameTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);

        unsigned int stencilDepthTexture;
        glGenTextures(1, &stencilDepthTexture);
        glBindTexture(GL_TEXTURE_2D, stencilDepthTexture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilDepthTexture, 0);       
        

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            std::cout << "frame buffer complete";
        else return 0;


        vector<glm::vec3> vegetation = {
            glm::vec3(1.0f, 0.0f, -5.0f),
            glm::vec3(-1.0f, 0.0f, -4.0f),
            glm::vec3(2.0f, 0.0f, -5.0f),
            glm::vec3(-1.0f, 0.0f, -3.0f)
        };

        Shader reflectionShader("reflectionVertex.glsl", "reflectionFragment.glsl", nullptr);

        GLuint uniformBuffer;
        glGenBuffers(1, &uniformBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
        
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

        GLuint cubeUniformBlock = glGetUniformBlockIndex(cubeShader.ID, "Matrices");
        GLuint mainUniformBlock = glGetUniformBlockIndex(ourShader.ID, "Matrices");
        GLuint outlineUniformBlockIndex = glGetUniformBlockIndex(outlineShader.ID, "Matrices");
        GLuint reflectionUniformBlockIndex = glGetUniformBlockIndex(reflectionShader.ID, "Matrices");
        GLuint geometryTesterUniformBlockIndex = glGetUniformBlockIndex(geometryTester.ID, "Matrices");

        glUniformBlockBinding(cubeShader.ID, cubeUniformBlock, 0);  
        glUniformBlockBinding(ourShader.ID, mainUniformBlock, 0);
        glUniformBlockBinding(outlineShader.ID, outlineUniformBlockIndex, 0);
        glUniformBlockBinding(reflectionShader.ID, reflectionUniformBlockIndex, 0);
        glUniformBlockBinding(geometryTester.ID, geometryTesterUniformBlockIndex, 0);   


        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBuffer, 0, 2 * sizeof(glm::mat4));  

        Shader rayShader = Shader("rayVertex.glsl", "rayFragment.glsl", nullptr);        

        float testVertices[] = {
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };

        GLuint sbo;
        glGenBuffers(1, &sbo);

        glBindBuffer(GL_SAMPLER_BUFFER, sbo);
        glBufferData(GL_SAMPLER_BUFFER, sizeof(testVertices), testVertices, GL_STATIC_DRAW);

        GLuint st;
        glGenTextures(1, &st);

        glBindTexture(GL_SAMPLER_BUFFER, 0);        

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);        

        Scene scene = Scene();
        input * inputobject = scene.getInputObject();
        
        bool dev = false, userinput = true;

        Shader depthShader("shadowMapVert.glsl", "shadowMapFrag.glsl", nullptr);




        unsigned int ShadowMapFrameBuffer; //creates the frame buffer for the shadow map
        glGenFramebuffers(1, &ShadowMapFrameBuffer);
        //create depth texture
        unsigned int shadowWidth = 1024, shadowHeight = 1024;
        unsigned int shadowMap;
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //attaching the depth texture to the buffer
        glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        while (!glfwWindowShouldClose(window)) {  

            if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
                dev = true;
                userinput = false;
            }
            else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
                dev = false;
                userinput = true;
            }

            inputobject->handleInput(window, userinput, dev);            

            glEnable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                      
            

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.0f, 0.0f, 0.03f, 1.0f);

            //glfwSetCursorPosCallback(window, cursor_position_callback);            

            //clear the color from the previous frame
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);       

            //input
            //processInput(window, &vertical, &horizontal, &spinx, &spiny, &spinz, characterState);

            //view and projection transforms
            glm::mat4 view = inputobject->getViewMatrix(); 
            
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(inputobject->getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);                       
            
            
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);                      
            

            //diver->rotate(1000000000000000000.0f);
            
            /*
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                model1->transformModel( glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.001f, 0.0f)), true);
            } 
            */
            
            glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
            glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindVertexArray(VAO1);
            
            reflectionShader.use();

            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

            GLuint reflectionModel = glGetUniformLocation(reflectionShader.ID, "model");

            glUniformMatrix4fv(reflectionModel, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f))));
            reflectionShader.setVec3("cameraPos", inputobject->getCameraPosition());

            glDrawArrays(GL_TRIANGLES, 0, 36);            

            //rendering commands      
            glBindVertexArray(VAO1);
            ourShader.use();

            //uniforms for shader 1
            GLuint modelUniform = glGetUniformLocation(ourShader.ID, "model");    

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            

            //model1.transformModel(model);

            //std::cout << model1.doesCollide(model2);
            //model1.doesCollide(model2);
            
            GLuint colorUniform = glGetUniformLocation(ourShader.ID, "ourColor");

            //setup light
            glm::vec3 lightPosition = glm::vec3(3.0f, 3.0f, 0.0f);
            glm::vec3 lightAmbient = glm::vec3(0.05f);
            glm::vec3 lightDiffuse = glm::vec3(1.0f);
            glm::vec3 lightSpecular = glm::vec3(1.0f);

            ourShader.setVec3("viewPos", inputobject->getCameraPosition());

            ourShader.setVec3("lightPos", glm::vec3(lightPosition));
            ourShader.setVec3("light.ambient", glm::vec3(lightAmbient));
            ourShader.setVec3("light.diffuse", glm::vec3(lightDiffuse));
            ourShader.setVec3("light.specular", glm::vec3(lightSpecular));
            ourShader.setFloat("light.quadratic", 0.0007f);
            ourShader.setFloat("light.linear", 0.14f);
            ourShader.setFloat("light.constant", 1.0f);

            glBindTexture(GL_TEXTURE_2D, fish_scales);
            //ourModel.Draw(ourShader);            

            model = glm::mat4(1.0f);
            
            glBindVertexArray(VAO1);
            ourShader.use();

            glDisable(GL_CULL_FACE);

            scene.drawScene(ourShader, {background, fish_scales, texture});

            glDisable(GL_CULL_FACE);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));           

            outlineShader.use();

            glBindVertexArray(lightVAO);
            //glBindTexture(GL_TEXTURE_2D, background);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition));            

            GLint outlineModel = glGetUniformLocation(outlineShader.ID, "model");

            glUniformMatrix4fv(outlineModel, 1, GL_FALSE, glm::value_ptr(model));
            //glDepthMask(GL_FALSE);

            glStencilMask(0xFF);

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            ourShader.use();

            glUniform4f(colorUniform, 1.0f, 1.0f, 1.0f, 1.0f);

            model = glm::translate(glm::identity<glm::mat4>(), glm::vec3(lightPosition));

            model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
            
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            //glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
            glUniform4f(colorUniform, 0.0f, 1.0f, 1.0f, 1.0f);
            //check call events and swap buffers            

            glBindVertexArray(vegetationVao);
            outlineShader.use();
            
            for (int i = 0; i < 100; i++) {
                model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::translate(model, glm::vec3(50* cos(10 * i), 0.0f, 25 * sin(i)));
                glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
                glBindTexture(GL_TEXTURE_2D, grass);
                glDrawArrays(GL_TRIANGLES, 6, 6);
            }     

            //|||||||||||||||||||Geometry Shader|||||||||||||||||||||||||

            glBindVertexArray(VAO1);    

            geometryTester.use();
            geometryTester.setMat4("model", glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 2.0f, -1.0f))));
            geometryTester.setFloat("time", glfwGetTime());

            ourModel->Draw(geometryTester, true);

            ourShader.use();
            ourShader.setMat4("model", glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 2.0f, -1.0f))));
            glBindTexture(GL_TEXTURE_2D, fish_scales);

            ourModel->Draw(ourShader, true);

            //glDrawArrays(GL_TRIANGLES, 0, 36);


            //||||||||||||||||||||||||||||||||||||||||
            
            //|||||||||| Rendering Shadows |||||||||||
            
            
            glm::mat4 lightProjection = glm::ortho(-35.0f, 35.0f, 35.0f, 35.0f, 0.1f, 75.0f);
            glm::mat4 lightView = glm::lookAt(20.0f * lightLocation, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            depthShader.use();
            depthShader.setMat5("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, shadowHeight, shadowWidth);
            glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFrameBuffer);
            glClear(GL_DEPTH_BUFFER_BIT);
           // Scene(depthShader);
            //Scene::drawScene()
            

            //||||||||||||||||||||||||||||||||||||||||


            glBindVertexArray(skyvao);
            cubeShader.use();
            /*
            GLuint skyviewUniform = glGetUniformLocation(cubeShader.ID, "view");
            GLuint skyprojectionUniform = glGetUniformLocation(cubeShader.ID, "projection");

            glUniformMatrix4fv(skyviewUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view))));
            glUniformMatrix4fv(skyprojectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
            */

            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
            

            //glDepthMask(GL_FALSE);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDepthMask(GL_TRUE);

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindVertexArray(vegetationVao);
            outlineShader.use();
            model = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 1.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -50.0f));
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, blending_window);
            glDrawArrays(GL_TRIANGLES, 6, 6);

            glDisable(GL_BLEND);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(QUADVAO);

            modelUniform = glGetUniformLocation(quadShader.ID, "model");
            
            quadShader.use();
            
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));
            
            glBindTexture(GL_TEXTURE_2D, frameTexture);
            glDisable(GL_DEPTH_TEST);
            glDrawArrays(GL_TRIANGLES, 0, 6);   


            //write to the main buffer
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClearDepth(1.0f); // set the clear value for the depth buffer, 1.0f is the default clear value
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the depth buffer and color buffer

            glBindVertexArray(QUADVAO);

            glEnable(GL_DEPTH_TEST); // enable depth testing
            glDepthMask(GL_TRUE); // ensure writing the depth buffer is written to
            glDepthFunc(GL_LEQUAL); // equal, lequal, greater, gequal, equal
            glDisable(GL_STENCIL_TEST); //explicitly disable stencil testing

            rayShader.use();
            rayShader.setFloat("triangle.vertex1", 0.5f);
            rayShader.setFloat("triangle.vertex2", 0.5f);
            rayShader.setFloat("triangle.vertex3", 0.5f);

            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_SAMPLER_BUFFER, st);
            glBindBuffer(GL_SAMPLER_BUFFER, sbo);
            glTexBuffer(GL_SAMPLER_BUFFER, GL_RGB, sbo);

            GLuint triangleData = glGetUniformLocation(rayShader.ID, "triangleData");

            glUniform1i(triangleData, st);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(VAO1);
            glBindTexture(GL_TEXTURE_2D, texture);

            //ourShader.use();
            //glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
};
#endif