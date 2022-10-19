#include "PhysicsModel.h";
#include "vector";

float PhysicsModel::resistanceCoef = 0.01f;
std::vector<PhysicsModel*> PhysicsModel::collisionModels = std::vector<PhysicsModel*>();
float PhysicsModel::GRAVITYPOWER = 3.0f;