#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__
#include <glm/vec2.hpp>

struct Transform
{
	glm::vec2 position;
	float rotation;			// Changed for Assignment 2
	glm::vec2 scale;
};
#endif /* defined (__TRANSFORM__) */