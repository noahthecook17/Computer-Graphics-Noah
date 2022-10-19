#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "Shader_H.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "physics.h"
#include "BoundingBox.h"
#include "OBB.h"
#include <glm/gtx/string_cast.hpp>
#include "PhysicsModel.h"
#include "modelLoader.h"
#include "input.h"

#ifndef SCENE_H
#define SCENE_H

class Scene {
private:

	vector<Model*> models;
    input * inputobject;

public:

	Scene() {

        models = vector<Model*>();

        Model * diverModel = new Model("BetterDiver.obj");

        diverModel->initialise(90, 0, -90, AXIS_Z);

        PhysicsModel* diver = diverModel->getTestBox(true, true);
        diver->translateModel(glm::vec3(-5.0f, 30.0f, 0.0f));

        diver->setDefaults(glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec3(90, 0, -90));

        Model * floorModel = new Model("floor.obj");


        floorModel->initialise(0, 0, 0, AXIS_Z, glm::vec3(0.0f, 0.0f, 0.0f), 30.0f);
        

        PhysicsModel* floor = floorModel->getTestBox(false, true);     
        floor->setDefaults(glm::vec3(0.0f), glm::vec3(0.0f));

        Model * cubeModel = new Model("badcube.obj");

        cubeModel->initialise(0, 0, 0, AXIS_Z, glm::vec3(0.0f, 30.0f, 0.0f), 1.0f);

        PhysicsModel* cube1 = cubeModel->getTestBox(true, true);
        PhysicsModel* cube2 = cubeModel->getTestBox(true, true);
        cube2->translateModel(glm::vec3(10.0f, 0.0f, 0.0f));

        cube1->setDefaults(glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f));
        cube2->setDefaults(glm::vec3(10.0f, 6.0f, 0.0f), glm::vec3(0.0f));

        Model * ourModel = new Model("fish_1.obj");

        ourModel->initialise(0, 45.0f, 0, AXIS_RIGHT, glm::vec3(-10.0f, 5.0f, -20.0f), 0.2f);

        PhysicsModel* fishy1 = ourModel->getTestBox(false, true);
        PhysicsModel* fishy2 = ourModel->getTestBox(false, true);

        fishy1->setDefaults(glm::vec3(-10.0f, 5.0f, -20.0f), glm::vec3(0.0f, 45.0f, 0.0f));
        fishy2->setDefaults(glm::vec3(0.0f, 5.0f, -25.0f), glm::vec3(0.0f, 45.0f, 0.0f));
        fishy2->reset();

        inputobject = new input(diver);

        models.push_back(ourModel);
        models.push_back(cubeModel);
        models.push_back(diverModel);
        models.push_back(floorModel);

	}

    input* getInputObject() {
        return inputobject;
    }

    void drawScene(Shader& ourShader, vector<GLuint> textures) {

        PhysicsModel::updateAll();

        //should move the floor based on the position of the diver

        glBindTexture(GL_TEXTURE_2D, textures[0]);
        models[3]->Draw(ourShader, false); //floor

        glBindTexture(GL_TEXTURE_2D, textures[1]);
        models[0]->updateAllModels();
        models[0]->Draw(ourShader, false); //fish
        models[2]->Draw(ourShader, false); //diver

        glBindTexture(GL_TEXTURE_2D, textures[2]);

        models[1]->Draw(ourShader, false); //cube
       
    }
};

#endif

