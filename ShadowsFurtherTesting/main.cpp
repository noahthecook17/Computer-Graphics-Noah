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


const char* vertexShaderSource =
"#version 330 core\n"

"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"

"out vec3 ourColor;"

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"

"out vec4 FragColor;\n"

"uniform vec4 ourColor;\n"

"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\0";

const char* fragmentShaderSource2 =
"#version 330 core\n"

"in vec3 ourColor;\n"

"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\0";



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

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



    GLfloat lightVertices[] =
    { //     COORDINATES     //
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };

    GLuint lightIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };


    unsigned int texture;
    unsigned int background;
    glGenTextures(1, &texture);
    glGenTextures(1, &background);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

    data = stbi_load("background.png", &width, &height, &nrChannels, 0);

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

    //this is used for input to the vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Element Buffer Object
    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //uses uniforms to do transformations in the vertex shader and color in the fragment shader
    Shader ourShader("shader.vs", "shader.fs");

    Shader ourShader2("shader.vs", "shader.fs");
    glViewport(0, 0, 800, 600);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    float count = 0.0f;
    float fall = 2.0f;
    float spinx = 0.0f;
    float spiny = 0.0f;
    float spinz = 0.0f;
    float* test = new float(2.2f);
    std::cout << *test;
    std::cout << '\n';
    *test += 3.3f;
    float* test2 = test;
    *test2 += *test;
    std::cout << *test;
    while (!glfwWindowShouldClose(window)) {
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;

        float speed = 0.1f * deltaTime;
        //clear the color from the previous frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //input
        processInput(window);

        //transformation matrices
       /* glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(spinx), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(spiny), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(spinz), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 view = glm::mat4(1.0f);
        if (fall > -1.0f)
            fall -= 0.001f;
        view = glm::translate(view, glm::vec3(count, fall, -10.0f));


        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            count += 0.001f * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            count -= 0.001f * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            spinx += 0.1f * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            spiny += 0.1f * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            spinz += 0.1f * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            count = 0.0f;
            fall = 2.0f;
            spinx = 0.0f;
            spiny = 0.0f;
            spinz = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            fall += 1.0f * speed;
        }

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);*/


        //rendering commands      
        glBindVertexArray(VAO1);
        ourShader.use();

        //GLuint modelUniform = glGetUniformLocation(ourShader.ID, "model");
        //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        //GLuint viewUniform = glGetUniformLocation(ourShader.ID, "view");
        //glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        //GLuint projectionUniform = glGetUniformLocation(ourShader.ID, "projection");
        //glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint colorUniform = glGetUniformLocation(ourShader.ID, "ourColor");
        //glUniform4f(colorUniform, 0.5f, 0.35f, 0.05f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 0, 6); //top
        //glUniform4f(colorUniform, 0.5f, 0.35f, 0.05f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 6, 6); //bottom
        //glUniform4f(colorUniform, 1.0f, 0.5f, 0.0f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 12, 6); //front
        //glUniform4f(colorUniform, 1.0f, 0.5f, 0.0f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 18, 6); //front
        //glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 24, 6); //back
        //glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
        //glDrawArrays(GL_TRIANGLES, 30, 6); //back
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //modelUniform = glGetUniformLocation(ourShader.ID, "model");
        //glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

        //view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        //viewUniform = glGetUniformLocation(ourShader.ID, "view");
        //glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

        //projectionUniform = glGetUniformLocation(ourShader.ID, "projection");
        //glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO1);

        glBindTexture(GL_TEXTURE_2D, background);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);        

        //check call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}