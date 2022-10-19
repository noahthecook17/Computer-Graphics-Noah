#pragma once
#ifndef program2
#define program2
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


class Program2{   

    
public:
    static int run() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "Collision Detection", NULL, NULL);

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

        float object1v[] = {
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        
        GLuint VBO;
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(VBO, sizeof(object1v), &object1v, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);

        glViewport(0, 0, 800, 600);

        glClearColor(0.0f, 0.0f, 5.0f, 1.0f);

        Shader ourShader("vertex.glsl", "fragment.glsl", nullptr);

        while (true) {

            glClear(GL_COLOR_BUFFER_BIT);

            ourShader.use();

            glBindVertexArray(VAO);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
};
#endif