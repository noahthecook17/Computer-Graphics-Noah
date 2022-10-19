#pragma once
#include "Model.h"
class Moving_Model :
    public Model
{
private:
    float velocity;
    float acceleration;
public:
    Moving_Model(float vertices[], int size, float velocity, bool gravity);
};

