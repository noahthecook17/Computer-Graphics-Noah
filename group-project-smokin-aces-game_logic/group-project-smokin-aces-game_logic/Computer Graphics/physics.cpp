#define PHYSICS_IMPLEMENTATION
#include "physics.h"

//graphics language framework
#include <GLFW/glfw3.h>

#include <stdio.h>  
#include <stdlib.h>  
#include <chrono>

#include <vector>

#include <iostream>


physics::physics(float vertices[], int size) {
	

	bool firstRun = true;

	physics::transform = glm::mat4(1.0f);

	min = glm::vec3(1.0f, 1.0f, 1.0f);
	max = glm::vec3(-1.0f, -1.0f, -1.0f);

	velocity = glm::vec2(0.0f, 0.0f);
	lastVelocityUpdate = glfwGetTime();
	acceleration = glm::vec2(0.0f, 0.0f);

	//calculate bounding box
	for (int i = 0; i < size; i++) {
		//std::cout << "here: ";
		//std::cout << sizeof(vertices) / sizeof(vertices[0]);

		if (i % 8 == 0) {
			if (firstRun) {
				min.x = vertices[i];
				max.x = vertices[i];
			}
			else {
				if (vertices[i] < min.x) {
					min.x = vertices[i];
				}
				if (vertices[i] > max.x) {
					max.x = vertices[i];
				}
			}
		}
		else if (i % 8 == 1) {
			if (firstRun) {
				min.y = vertices[i];
				max.y = vertices[i];
			}
			else if (vertices[i] < min.y) {
				min.y = vertices[i];
			}
			else if (vertices[i] > max.y) {
				max.y = vertices[i];
			}
		}
		else if (i % 8 == 2) {
			if (firstRun) {
				min.z = vertices[i];
				max.z = vertices[i];
				firstRun = false;
			}
			else if (vertices[i] < min.z) {
				min.z = vertices[i];
			}
			else if (vertices[i] > max.z) {
				max.z = vertices[i];
			}
		}
	}
	//std::cout << min.x;
	//std::cout << max.x;

};

physics::physics(glm::vec3 vertices[]) {

	bool firstRun = true;

	physics::transform = glm::mat4(1.0f);

	min = glm::vec3(1.0f, 1.0f, 1.0f);
	max = glm::vec3(-1.0f, -1.0f, -1.0f);

	velocity = glm::vec2(0.0f, 0.0f);
	lastVelocityUpdate = glfwGetTime();
	acceleration = glm::vec2(0.0f, 0.0f);

	//calculate bounding box
	for (int i = 0; i < 20; i++) {

		if (firstRun) {
			min.x = vertices[i].x;
			max.x = vertices[i].x;
			min.y = vertices[i].y;
			max.y = vertices[i].y;
			min.z = vertices[i].z;
			max.z = vertices[i].z;
			firstRun = false;
		}
		if (vertices[i].x < min.x) {
			min.x = vertices[i].x;
		}
		if (vertices[i].x > max.x) {
			max.x = vertices[i].x;
		}
		if (vertices[i].y < min.y) {
			min.y = vertices[i].y;
		}
		if (vertices[i].y > max.y) {
			max.y = vertices[i].y;
		}
		if (vertices[i].z < min.z) {
			min.z = vertices[i].z;
		}
		if (vertices[i].z > max.z) {
			max.z = vertices[i].z;
		}		
	}
};

void physics::doTransform() {
	min = transform * glm::vec4(min, 1.0f);
	max = transform * glm::vec4(max, 1.0f);
}

void physics::setAcceleration(glm::vec2 acceleration) {
	this->acceleration = acceleration;
}

glm::mat4 physics::getMovementTransform() {
	//update velocity based on acceleration
	double deltaTime = glfwGetTime() - lastVelocityUpdate;
	glm::vec2 displacement = velocity * (float)deltaTime;
	//std::cout << displacement.x;
	velocity += (float)(deltaTime)*acceleration;
	lastVelocityUpdate += deltaTime;
	//std::cout << displacement.x;
	return glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, displacement.y, 0.0f));
}

void physics::updateLocation(physics* physics[], int size) {
	glm::mat4 transform = getMovementTransform();
	glm::vec4 testMin = (transform * glm::vec4(min, 1.0f));
	glm::vec4 testMax = (transform * glm::vec4(max, 1.0f));


	for (int i = 0; i < size; i++) {
		if (this != physics[i] && this->doesCollideOld(*physics[i], testMin, testMax)) {
			std::cout << "collision";
			//change acceleration vector
			//use direction vector?
			glm::vec3 direction = this->getDirection(*physics[i]);
			//maybe add the weight of the velocity? using dot product
			velocity = glm::vec2(0.0f, 0.0f);
			acceleration = glm::vec2(0.0f, 0.0f);
			return;
		}
	}

	min = testMin;
	max = testMax;
	this->transform = this->transform * transform;
}

void physics::transformModel(physics* physics[], int size, glm::mat4 transform) {
	glm::vec4 testMin = transform * glm::vec4(min, 1.0f);
	glm::vec4 testMax = transform * glm::vec4(max, 1.0f);

	for (int i = 0; i < size; i++) {
		if (this != physics[i] && this->doesCollideOld(*physics[i], testMin, testMax)) {
			return;
		}
	}

	min = testMin;
	max = testMax;
	this->transform = this->transform * transform;
}

void physics::transformModel(glm::mat4 transform) {
	min = transform * glm::vec4(min, 1.0f);
	max = transform * glm::vec4(max, 1.0f);
	//std::cout << min.x;
	//std::cout << max.x;
	this->transform = this->transform * transform;
}

glm::mat4 physics::getTransform() {
	return transform;
}

//testing purposes
glm::vec3 physics::getMin() {
	return min;
}

glm::vec3 physics::getMax() {
	return max;
}

glm::vec3 physics::getDirection(physics physic) {
	return glm::normalize(this->getCenter() - physic.getCenter());
}

glm::vec3 physics::getCenter() {
	return 0.5f * (min + max);
}

bool physics::doesCollideOld(physics physics, glm::vec3 min, glm::vec3 max) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			//std::cout << transform[x][y];
		}
	}

	bool collisionX = physics.max.x < max.x&& physics.max.x > min.x || //model param is contained within this model on x-axis
		max.x < physics.max.x&& max.x > physics.min.x || //this model is contained within model param on x-axis
		physics.max.x == max.x && physics.min.x == min.x; //exact same location on x-axis

	bool collisionY = physics.max.y < max.y&& physics.max.y > min.y ||
		max.y < physics.max.y&& max.y > physics.min.y ||
		physics.max.y == max.y && physics.min.y == min.y;

	if (collisionX && collisionY) {

		//std::cout << "collision dected.";
		return true;
	}
	else {
		return false;
	}
}

bool physics::doesCollide(physics physics, glm::vec3 pathOfMin, glm::vec3 pathOfMax) {
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			//std::cout << transform[x][y];
		}
	}

	bool collisionX = physics.max.x < max.x&& physics.max.x > min.x || //model param is contained within this model on x-axis
		max.x < physics.max.x&& max.x > physics.min.x || //this model is contained within model param on x-axis
		physics.max.x == max.x && physics.min.x == min.x; //exact same location on x-axis

	bool collisionY = physics.max.y < max.y&& physics.max.y > min.y ||
		max.y < physics.max.y&& max.y > physics.min.y ||
		physics.max.y == max.y && physics.min.y == min.y;

	if (collisionX && collisionY) {

		//std::cout << "collision dected.";
		return true;
	}
	else {
		return false;
	}
}