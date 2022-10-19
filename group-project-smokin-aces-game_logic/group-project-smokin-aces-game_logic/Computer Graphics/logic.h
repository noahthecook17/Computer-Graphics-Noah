#include "PhysicsModel.h"

#pragma once
#ifndef LOGIC_H
#define LOGIC_H

class logic {

public:

	void move(PhysicsModel* model) {
		model->swimRight(100.0f);
		model->rotate(1000.0f);
	}

};

#endif