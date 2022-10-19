#ifndef Physics_H
#define Physics_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vector"
#include "mesh.h"
#include "OBB.h"
#include <GLFW/glfw3.h>

enum axis {
	AXIS_FORWARD,
	AXIS_UP,
	AXIS_RIGHT,
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

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

	axis rotationalAxis;	

	float rotationalVelocity;
	float rotationalAcceleration;

	//
	float lastVelocityUpdate;

	

	bool extraTest(glm::vec3 direction, bool gameOver) {		
		float velocityStrength = glm::dot(direction, velocity);

		if (velocityStrength > 0.0f) {
			OBB* copy = this->boundingBox->getCopy();
			copy->translate(direction * 0.01f);
			if (collisionTest(copy)) {
				if (gameOver) { 
					resetAll(); 
					return false;
				}
				else {
					velocity = velocity - (velocityStrength * direction);
				}
			}		
		}
		return true;
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
	

	void extraTests(bool gameOver) {

		if (!extraTest(glm::vec3(0.0f, -1.0f, 0.0f), gameOver)) return;

		if (!extraTest(this->boundingBox->getForward(), gameOver)) return;

		if (!extraTest(this->boundingBox->getRight(), gameOver)) return;

		if (!extraTest(this->boundingBox->getUp(), gameOver)) return;


		//if (!collisionTest(copy)) velocity  
	}

	static std::vector<PhysicsModel*> collisionModels;
	static float GRAVITYPOWER;
	static float resistanceCoef;

	void setDefaults(glm::vec3 position, glm::vec3 rotation) {
		this->boundingBox->setDefaults(position, rotation);
	}

	void reset() {
		float currentTime = glfwGetTime();

		this->boundingBox->reset();
		if (gravity)
			this->acceleration = glm::vec3(0.0, -GRAVITYPOWER, 0.0f);
		else
			this->acceleration = glm::vec3(0.0f);
		this->velocity = glm::vec3(0.0f);
		this->lastMovementPower = 0.0f;
		this->lastMovementDirection = glm::vec3(0.0f);
		this->lastMovement = currentTime;

		this->rotationalVelocity = 0.0f;
		this->rotationalAcceleration = 0.0f;

		this->lastRotationPower = 0.0f;
		this->lastRotation = currentTime;

		//
		this->lastVelocityUpdate = currentTime;
	}

	void scaleForward(float factor) {
		boundingBox->scaleForward(factor);
	}

	//for testing
	OBB* getOBB() {
		return this->boundingBox;
	}

	PhysicsModel(std::vector<Mesh> meshes, bool gravity, bool collision) {

		float currentTime = glfwGetTime();
		this->gravity = gravity;

		this->rotationalAxis = AXIS_Y;

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

	static void resetAll() {
		for (int i = 0; i < collisionModels.size(); i++) {
			collisionModels[i]->reset();
		}
	}

	void setRotationalAxis(axis rotationalAxis) {
		this->rotationalAxis = rotationalAxis;
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

		extraTests(false);
		float rotation = rotationalVelocity * deltaTime;
		glm::vec3 displacement = velocity * deltaTime;

		OBB * copy = this->boundingBox->getCopy();
		copy->translate(displacement);

		if (!collisionTest(copy)) { 
			this->boundingBox->translate(displacement); 
		}

		copy = this->boundingBox->getCopy();
		//cout << glm::to_string(this->boundingBox->getForward());
		
		glm::vec3 axisOfRotation = getAxisOfRotation();

		copy->rotate(rotation, axisOfRotation);
		if (debug) {
			cout << "rotation " << rotation << '\n';
		}	

		if (!collisionTest(copy)) {
			
			this->boundingBox->rotate(rotation, axisOfRotation);
		}

		updateMovement();
		updateRotation();

		lastVelocityUpdate += deltaTime;		
	}

	glm::vec3 getAxisOfRotation() {
		switch (rotationalAxis) {
		case AXIS_UP:
			return this->boundingBox->getUp();
			break;
		case AXIS_FORWARD:
			return this->boundingBox->getForward();
			break;
		case AXIS_RIGHT:
			return this->boundingBox->getRight();
			break;
		case AXIS_X:
			return glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case AXIS_Y:
			return glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case AXIS_Z:
			return glm::vec3(0.0f, 0.0f, 1.0f);
			break;
		default:
			return glm::vec3(0.0f, 0.0f, 1.0f);
		}
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