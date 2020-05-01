#pragma once

#include <glm/glm.hpp>

#include "Stage.h"

class Ray {
public:
	Ray(Stage* setstage, glm::vec3 setposition, float& sethorizontal, float& setvertical, float setmaxlength);
	~Ray();

	bool Step();

	Stage* stage;
	glm::vec3 position;
	float horizontal;
	float vertical;
	float maxlength;
	float length;
};