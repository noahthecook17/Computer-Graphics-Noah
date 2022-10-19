#include <GLFW/glfw3.h>
#include "learnopengl_camera.h"
#include "PhysicsModel.h"

#ifndef INPUT_H
#define INPUT_H
class input {
private:
    float lastFrame;
    float lastYpos;
    float lastXpos;    

    Camera camera;
    PhysicsModel* diver;

public:

    input(PhysicsModel * diver) {
        lastFrame = glfwGetTime();
        lastYpos = 0.0f;
        lastXpos = 0.0f;
        this->camera = Camera();
        this->diver = diver;        
    }


	void handleInput(GLFWwindow* window, bool userInput, bool cameraInput) {

        float deltaTime = 0.0f;

        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;
        float speed = 4.0f * deltaTime;
           
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwDestroyWindow(window);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            PhysicsModel::resetAll();
        }

		if (userInput) {  

            glm::vec3 diverCenter = diver->getOBB()->getCenter();
            glm::vec3 cameraPosition = glm::vec3(diverCenter.x, diverCenter.y + 2.0f, diverCenter.z + 10.0f);

            camera.Up = glm::vec3(0, 1, 0);
            camera.Right = glm::vec3(0, 0, 1);

            camera.Position = cameraPosition;

            camera.Front = diverCenter - cameraPosition;

            diver->swimForward(100.0f);

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                diver->swimForward(100.0f);
            }
            float deltaY = lastYpos - ypos;
            if (deltaY != 0) {
                diver->rotate(deltaY * 10000000.0f);
                lastYpos = ypos;
                lastXpos = xpos;
            }

            diver->extraTests(true);

		}

        if (cameraInput) {
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

            camera.ProcessMouseMovement(xpos - lastXpos, lastYpos - ypos);
            lastXpos = xpos;
            lastYpos = ypos;
        }

	}

    glm::mat4 getViewMatrix() {
        return camera.GetViewMatrix();
    }

    float getZoom() {
        return camera.Zoom;
    }

    glm::vec3 getCameraPosition() {
        return camera.Position;
    }


};
#endif