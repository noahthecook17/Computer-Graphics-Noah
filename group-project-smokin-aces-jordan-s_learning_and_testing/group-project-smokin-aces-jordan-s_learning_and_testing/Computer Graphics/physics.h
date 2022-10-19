#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#ifndef PHYSICS_H
#define PHYSICS_H

class physics
{
private:	

	glm::vec3 min;
	glm::vec3 max;

	glm::mat4 transform;

	glm::vec2 velocity;
	float lastVelocityUpdate;

	glm::vec2 acceleration;

	void doTransform();

	glm::mat4 getMovementTransform();

	glm::vec3 getCenter();

	glm::vec3 getDirection(physics);

	//calculates
	bool doesCollide(physics, glm::vec3 pathOfMin, glm::vec3 pathOfMax);

public:

	physics(glm::vec3[]);

	physics(float[], int);

	//returns true if collision occurs on the x and y coordinates
	bool doesCollideOld(physics, glm::vec3, glm::vec3);

	//check for collision before doing transform
	void transformModel(physics* [], int size, glm::mat4);

	//transform the object in the model space
	void transformModel(glm::mat4);

	//get the model space transform for this object
	glm::mat4 getTransform();

	//set acceleration
	void setAcceleration(glm::vec2);

	//calculates movement of object based on its acceleration and velocity
	void updateLocation(physics* [], int);

	//to test stuff
	glm::vec3 getMin();

	//to test stuff
	glm::vec3 getMax();
};

#endif

