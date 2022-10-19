#include "Moving_Model.h"


Moving_Model::Moving_Model(float vertices[], int size, float velocity, bool gravity) : Model(vertices, size) {
	if (gravity) {
		acceleration = -0.001f;
	}
	else acceleration = 0.0f;
	this->velocity = velocity;
}