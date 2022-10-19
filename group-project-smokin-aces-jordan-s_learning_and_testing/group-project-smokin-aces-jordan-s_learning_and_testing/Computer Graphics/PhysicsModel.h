#ifndef Physics_H
#define Physics_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vector"
#include "mesh.h"
#include "OBB.h"
#include <GLFW/glfw3.h>



class PhysicsModel{	
private:
	OBB* boundingBox;
	//		

	//used in the update locations function
	bool gravity;

	float lastMovementPower;
	glm::vec3 lastMovementDirection;
	float lastMovement;

	float lastRotationPower;
	float lastRotation;

	glm::vec3 acceleration;

	glm::vec3 velocity;

	glm::vec3 rotationalAxis;	

	float rotationalVelocity;
	float rotationalAcceleration;

	//
	float lastVelocityUpdate;

	void extraTests() {
		directionTest(glm::vec3(0.0f, -1.0f, 0.0f));

		directionTest(this->boundingBox->getForward());

		directionTest(this->boundingBox->getRight());

		directionTest(this->boundingBox->getUp());
		

		//if (!collisionTest(copy)) velocity  
	}

	void directionTest(glm::vec3 direction) {
		float velocityStrength = glm::dot(direction, velocity);

		if (velocityStrength > 0.0f) {
			OBB* copy = this->boundingBox->getCopy();
			copy->translate(direction * 0.01f);
			if (collisionTest(copy)) {
				velocity = velocity - (velocityStrength * direction);
			}		
		}
	}
	

	//tests the param for collisions with all models in the collisionModels
	bool collisionTest(OBB * copy) {
		for (int i = 0; i < collisionModels.size(); i++) {
			if (this->boundingBox != collisionModels[i]->boundingBox && copy->collidesWith(*collisionModels[i]->boundingBox)) {				
				return true;
			}
		}
		return false;
	}

	void updateMovement() {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastMovement;
		if (deltaTime > 0.01) {
			acceleration -= lastMovementDirection * lastMovementPower;
			lastMovementDirection = glm::vec3(0.0f);
			lastMovementPower = 0.0f;			
		}
	}

	void updateRotation() {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastMovement;
		if (deltaTime > 0.01) {
			rotationalAcceleration -= lastRotationPower;
			lastRotationPower = 0.0f;
		}
	}
	
public:

	static std::vector<PhysicsModel*> collisionModels;
	static float GRAVITYPOWER;
	static float resistanceCoef;

	

	//for testing
	OBB* getOBB() {
		return this->boundingBox;
	}

	PhysicsModel(std::vector<Mesh> meshes, bool gravity, bool collision) {

		float currentTime = glfwGetTime();
		this->gravity = gravity;

		this->rotationalAxis = glm::vec3(0.0f);

		rotationalAcceleration = 0.0f;
		rotationalVelocity = 0.0f;

		boundingBox = new OBB(meshes);
		if (collision) collisionModels.push_back(this);
		if (gravity) acceleration = glm::vec3(0.0f, -1.0f, 0.0f) * GRAVITYPOWER;
		else acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		lastVelocityUpdate = 0.0f;
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		lastMovementDirection = glm::vec3(0.0f);
		lastMovementPower = 0.0f;
		lastMovement = currentTime;

		lastRotation = 0.0f;
		lastRotationPower = 0.0f;
	}	

	void setRotationalAxis(glm::vec3 axis) {
		rotationalAxis = axis;
	}	

	void rotateModel(float degrees, glm::vec3 axis) {
		boundingBox->rotate(degrees, axis);
	}

	void translateModel(glm::vec3 translation) {
		boundingBox->translate(translation);
	}

	void scaleModel(float factor) {
		boundingBox->scale(factor);
	}

	void updatePhysics() {	

		bool debug = false;

		if (debug) {
			cout << "rotationalAcceleration: " << rotationalAcceleration << '\n';
			cout << "rotationalAxis " << glm::to_string(rotationalAxis) << '\n';
		}

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastVelocityUpdate;
		
		//check for collisions in front of and below
		//set velocity to 0 in those directions if there is a collision		
		
		velocity += acceleration * deltaTime;

		rotationalVelocity += rotationalAcceleration * deltaTime;


		float velocityStrength = glm::length(velocity);

		if (velocityStrength != 0.0f) {
			glm::vec3 unitVelocity = velocity / velocityStrength;
			velocity = velocity - ((float)glm::pow(velocityStrength, 2) * resistanceCoef * unitVelocity);
		}
		
		if (rotationalVelocity > 0) {
			if(debug)
			cout << "r velocity before: " << rotationalVelocity << '\n';
			rotationalVelocity = rotationalVelocity - (float)glm::pow(rotationalVelocity, 2) * 0.0001;
			if (debug)
			cout << "r velocity after: " << rotationalVelocity << '\n';
		}
		else if (rotationalVelocity < 0) {
			if (debug)
			cout << "r velocity before: " << rotationalVelocity << '\n';
			rotationalVelocity = rotationalVelocity + (float)glm::pow(rotationalVelocity, 2) * 0.0001;
			if (debug)
			cout << "r velocity after: " << rotationalVelocity << '\n';
		}
		
		

		if (debug) {
			cout << "rotational velocity " << rotationalVelocity << '\n';
		}

		extraTests();
		float rotation = rotationalVelocity * deltaTime;
		glm::vec3 displacement = velocity * deltaTime;

		OBB * copy = this->boundingBox->getCopy();
		copy->translate(displacement);

		if (!collisionTest(copy)) { 
			this->boundingBox->translate(displacement); 
		}

		copy = this->boundingBox->getCopy();
		copy->rotate(rotation, rotationalAxis);
		if (debug) {
			cout << "rotation " << rotation << '\n';
		}
		if (!collisionTest(copy)) {
			this->boundingBox->rotate(rotation, rotationalAxis);
		}

		updateMovement();
		updateRotation();

		lastVelocityUpdate += deltaTime;		
	}

	glm::mat4 getModelTransform() {
		//cout << "Model Transform: " << glm::to_string(boundingBox->getModelTransform()) << '\n';
		return boundingBox->getModelTransform();
	}
	

	//calls update location for every bounding box in the boundingBoxes
	static void updateAll() {

		for (int i = 0; i < collisionModels.size(); i++) {
			collisionModels[i]->updatePhysics();
		}

	}

	void swimForward(float power) {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastMovement;
		if (deltaTime > 0.1f) {
			lastMovementDirection = this->boundingBox->getUp();;
			lastMovementPower = GRAVITYPOWER * power;
			acceleration += lastMovementDirection * lastMovementPower;
			lastMovement = currentTime;
		}
	}

	void swimRight(float power) {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastMovement;
		if (deltaTime > 0.1f) {
			lastMovementDirection = this->boundingBox->getRight();;
			lastMovementPower = GRAVITYPOWER * power;
			acceleration += lastMovementDirection * lastMovementPower;
			lastMovement = currentTime;
		}
	}

	void rotate(float power) {
		bool debug = false;
		if (power > 1000.0f) power = 1000.0f;
		if (power < -1000.0f) power = -1000.0f;
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastRotation;
		if (deltaTime > 0.1f) {
			lastRotationPower = GRAVITYPOWER * power;
			if (rotationalAcceleration + lastMovementPower < 10000.0f && rotationalAcceleration + lastMovementPower > -10000.0f)
				rotationalAcceleration += lastRotationPower;
			if (debug) {
				cout << "input rotationalAcceleration " << rotationalAcceleration << '\n';
			}
			lastRotation = currentTime;
		}
	}
};

#endif