#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vector"
#include "mesh.h"

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

class BoundingBox {

private:
	glm::vec3 min; //bounding box
	glm::vec3 max; //bounding box

	glm::mat4 transform; //transform the bounding box

	//
	static float gravityPower;

	float resistancePower;

	//used in the update locations function
	bool gravity;

	float lastMovementPower;

	glm::vec3 lastMovementDirection;

	glm::vec3 acceleration;

	glm::vec3 velocity;

	float lastVelocityUpdate;

	float lastMovement;
	
	static std::vector<BoundingBox*> boundingBoxes;
	

	void doTransform();

	//updates the velocity of the object and then returns a transform for the displacement of the object
	glm::mat4 getMovementTransform();	
	
	bool gravityEnabled;

	//glm::vec3 getDirection(BoundingBox);
	bool doesCollideOld(BoundingBox, glm::vec3, glm::vec3);
	

public:
	void reset();

	glm::vec3 getCenter();

	//updates the location of this bounding box based on its acceleration and whether gravity should be applied to it or not.
	void updateLocation();

	//calls update location for every bounding box in the boundingBoxes
	static void updateLocations();

	void increaseAcceleration(glm::vec3);

	BoundingBox(float[], int, bool);

	BoundingBox(std::vector<Mesh>, bool);

	void movement(glm::vec3, float);

	void swim(glm::vec3, float);

	//transform the object in the model space
	void transformModel(glm::mat4, bool);

	void setAcceleration(glm::vec3);

	//get the model space transform for this object
	glm::mat4 getTransform();	

	//to test stuff
	glm::vec3 getMin();

	//to test stuff
	glm::vec3 getMax();
};

#endif
