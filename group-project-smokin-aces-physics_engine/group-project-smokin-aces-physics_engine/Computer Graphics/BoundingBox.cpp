#include "BoundingBox.h"
#include <GLFW/glfw3.h>
#include "vector"

#include <stdio.h>  
#include <stdlib.h>  
#include <chrono>
#include <iostream>



std::vector<BoundingBox*> BoundingBox::boundingBoxes = vector<BoundingBox*>();
float BoundingBox::gravityPower = 3.00000000000000000000000000000000000000000000000f;

BoundingBox::BoundingBox(float vertices[], int size, bool gravity) {
	lastVelocityUpdate = 0.0f;
	bool firstRun = true;	
	lastMovement = (float)glfwGetTime();
	lastMovementDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	lastMovementPower = 0.0f;
	this->gravity = gravity;
	this->gravityEnabled = gravity;

	BoundingBox::transform = glm::mat4(1.0f);	

	min = glm::vec3(1.0f, 1.0f, 1.0f);
	max = glm::vec3(-1.0f, -1.0f, -1.0f);

	if (gravity) 
		acceleration = glm::vec3(0.0f, -gravityPower, 0.0f);
	
	else
		acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

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
	boundingBoxes.push_back(this);
	/*
	for (int i = 0; i < boundingBoxes.size(); i++) {
		cout << boundingBoxes[i]->min.x;
		cout << boundingBoxes[i]->max.x;
	}
	*/
};


BoundingBox::BoundingBox(std::vector<Mesh> meshes, bool gravity) {
	lastVelocityUpdate = 0.0f;
	bool firstRun = true;
	lastMovement = (float)glfwGetTime();
	lastMovementDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	lastMovementPower = 0.0f;
	this->gravity = gravity;
	this->gravityEnabled = gravity;

	BoundingBox::transform = glm::mat4(1.0f);

	min = glm::vec3(1.0f, 1.0f, 1.0f);
	max = glm::vec3(-1.0f, -1.0f, -1.0f);

	if (gravity)
		acceleration = glm::vec3(0.0f, -gravityPower, 0.0f);

	else
		acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	if (gravity)
		acceleration = glm::vec3(0.0f, -3.00000000000000000000000000000000000000000000000f, 0.0f);
	else
		acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	//if (vertices.size() != 1) throw new exception();
	for (int i = 0;i < meshes.size(); i++) {
		vector<Vertex> vertices = meshes[i].vertices;
		for (int i = 0; i < vertices.size(); i++) {
			//cout << vertices[i].Position.x;
			//cout << vertices[i].Position.y;
			//cout << vertices[i].Position.z;
			if (firstRun) {
				min = vertices[i].Position;
				max = vertices[i].Position;
				firstRun = false;
			}
			else {
				if (min.x > vertices[i].Position.x)
					min.x = vertices[i].Position.x;
				if (min.y > vertices[i].Position.y)
					min.y = vertices[i].Position.y;
				if (min.z > vertices[i].Position.z)
					min.z = vertices[i].Position.z;
				if (max.x < vertices[i].Position.x)
					max.x = vertices[i].Position.x;
				if (max.y < vertices[i].Position.y)
					max.y = vertices[i].Position.y;
				if (max.z < vertices[i].Position.z)
					max.z = vertices[i].Position.z;
			}
		}
	}
	cout << min.x;
	cout << min.y;
	cout << min.z;
	cout << max.x;
	cout << max.y;
	cout << max.z;
	boundingBoxes.push_back(this);
}

void BoundingBox::doTransform() {
	min = transform * glm::vec4(min, 1.0f);
	max = transform * glm::vec4(max, 1.0f);
}

void BoundingBox::updateLocations() {
	//cout << boundingBoxes.size();
	for (int i = 0; i < boundingBoxes.size(); i++) {
		
		boundingBoxes[i]->updateLocation();
	}
}

void BoundingBox::movement(glm::vec3 direction, float power) {
	float currentTime = glfwGetTime();
	for (int i = 0; i < boundingBoxes.size(); i++) {
		if (this != boundingBoxes[i] && this->doesCollideOld(*boundingBoxes[i], glm::vec3(min.x, min.y - 0.1f, min.z), glm::vec3(max.x, max.y - 0.1f, max.z))) {
			if ((currentTime - lastMovement) > 0.1f) {
				lastMovement = currentTime;
				lastMovementDirection = glm::normalize(direction);
				lastMovementPower = power * gravityPower;
				acceleration += lastMovementDirection * lastMovementPower;
			}
			return;
		}
	}
	
}

glm::mat4 BoundingBox::getMovementTransform() {
	float currentTime = glfwGetTime();
	if ((currentTime - lastMovement) > 0.01f) {		
		acceleration -= lastMovementDirection * lastMovementPower;	
		lastMovementDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		lastMovementPower = 0.0f;
	}
	if (gravity) {
		for (int i = 0; i < boundingBoxes.size(); i++) {
			if (this != boundingBoxes[i] && this->doesCollideOld(*boundingBoxes[i], glm::vec3(min.x, min.y - 0.1f, min.z), glm::vec3(max.x, max.y - 0.1f, max.z))) {
				if (gravityEnabled) {
					acceleration = glm::vec3(acceleration.x, acceleration.y + gravityPower, acceleration.z);
					gravityEnabled = false;
				}
				//update velocity based on acceleration
				double deltaTime = currentTime - lastVelocityUpdate;
				glm::vec3 displacement = velocity * (float)deltaTime;
				velocity += (float)(deltaTime)*acceleration;
				lastVelocityUpdate += deltaTime;
				return glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, displacement.y, displacement.z));
			}
		}
		if (acceleration.y > -gravityPower && !gravityEnabled) {
			acceleration = glm::vec3(acceleration.x, acceleration.y - gravityPower, acceleration.z);
			gravityEnabled = true;
		}
		//update velocity based on acceleration
		double deltaTime = currentTime - lastVelocityUpdate;
		glm::vec3 displacement = velocity * (float)deltaTime;
		velocity += (float)(deltaTime)*acceleration;
		lastVelocityUpdate += deltaTime;
		return glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, displacement.y, displacement.z));
	}
	double deltaTime = currentTime - lastVelocityUpdate;
	glm::vec3 displacement = velocity * (float)deltaTime;
	velocity += (float)(deltaTime)*acceleration;
	lastVelocityUpdate += deltaTime;
	return glm::translate(glm::mat4(1.0f), glm::vec3(displacement.x, displacement.y, displacement.z));
}

//use this to update the position of elements based on the acceleration and velocity of the object
void BoundingBox::updateLocation() {
	glm::mat4 displacement = getMovementTransform();
	glm::vec4 testMin = (displacement * glm::vec4(min, 1.0f));
	glm::vec4 testMax = (displacement * glm::vec4(max, 1.0f));
	//cout << boundingBoxes.size();

	for (int i = 0; i < boundingBoxes.size(); i++) {
		if (this != boundingBoxes[i] && this->doesCollideOld(*boundingBoxes[i], testMin, testMax)) {
			std::cout << "collision";
			//change acceleration vector
			//use direction vector?
			//glm::vec3 direction = this->getDirection(*models[i]);
			//maybe add the weight of the velocity? using dot product
			velocity = glm::vec3(0.0f, 0.0f, 0.0f);

			//acceleration = glm::vec2(0.0f, 0.0f);
			return;
		}
	}
	this->min = testMin;
	this->max = testMax;
	this->transform = this->transform * displacement;
}

void BoundingBox::transformModel(glm::mat4 transform, bool collisionDetection) {

	glm::vec4 testMin = transform * glm::vec4(min, 1.0f);
	glm::vec4 testMax = transform * glm::vec4(max, 1.0f);

	if (collisionDetection) {

		for (int i = 0; i < boundingBoxes.size(); i++) {
			if (this != boundingBoxes[i] && this->doesCollideOld(*boundingBoxes[i], testMin, testMax)) {
				cout << "collision";
				return;
			}
		}
		min = testMin;
		max = testMax;
		this->transform = this->transform * transform;		
	}
	else {
		//min = transform * glm::vec4(min, 1.0f);
		//max = transform * glm::vec4(max, 1.0f);
		//std::cout << min.x;
		//std::cout << max.x;
		min = testMin;
		max = testMax;
		this->transform = this->transform * transform;
	}
}

glm::mat4 BoundingBox::getTransform() {
	return transform;
}

//testing purposes
glm::vec3 BoundingBox::getMin() {
	return min;
}

glm::vec3 BoundingBox::getMax() {
	return max;
}

/*
glm::vec3 BoundingBox::getDirection(BoundingBox model) {
	return glm::normalize(this->getCenter() - model.getCenter());
}
*/

void BoundingBox::reset() {
	glm::mat4 inverse = glm::inverse(transform);
	min = inverse * glm::vec4(min, 1.0f);
	max = inverse * glm::vec4(max, 1.0f);

	lastVelocityUpdate = (float)glfwGetTime();
	lastMovement = (float)glfwGetTime();
	lastMovementDirection = glm::vec3(0.0f, 0.0f,0.0f);
	lastMovementPower = 0.0f;

	this->gravityEnabled = gravity;

	transform = glm::mat4(1.0f);

	if (gravity)
		acceleration = glm::vec3(0.0f, -gravityPower, 0.0f);

	else
		acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 BoundingBox::getCenter() {
	return 0.5f * (min + max);
}


void BoundingBox::setAcceleration(glm::vec3 acceleration) {
	this->acceleration = acceleration;
}

void BoundingBox::increaseAcceleration(glm::vec3 acceleration) {
	this->acceleration = this->acceleration + (acceleration * gravityPower/3.0f);
}

bool BoundingBox::doesCollideOld(BoundingBox model, glm::vec3 min, glm::vec3 max) {	

	bool collisionX = model.max.x < max.x&& model.max.x > min.x || //model param is contained within this model on x-axis
		max.x < model.max.x&& max.x > model.min.x || //this model is contained within model param on x-axis
		model.max.x == max.x && model.min.x == min.x; //exact same location on x-axis

	bool collisionY = model.max.y < max.y&& model.max.y > min.y ||
		max.y < model.max.y&& max.y > model.min.y ||
		model.max.y == max.y && model.min.y == min.y;

	bool collisionZ = model.max.z < max.z&& model.max.z > min.z ||
		max.z < model.max.z&& max.z > model.min.z ||
		model.max.z == max.z && model.min.z == min.z;

	if (collisionX && collisionY && collisionZ) {

		//std::cout << "collision dected.";
		return true;
	}
	else {
		return false;
	}
}
