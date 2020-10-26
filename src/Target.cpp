#include "Target.h"
#include "TextureManager.h"



Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/box.png","box");

	const auto size = TextureManager::Instance()->getTextureSize("box");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("box", x, y, getTransform()->rotation, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::doThrow()		// Assignment 1
{
	getTransform()->position = throwPosition;
	getRigidBody()->velocity = throwSpeed;
}

void Target::m_move()
{
	float deltaTime = 1.0f / 15.0f;
	glm::vec2 gravity = glm::vec2(0, 9.8f);
	if (getTransform()->position.y <= 550) 
	{
		getRigidBody()->velocity += getRigidBody()->acceleration * (deltaTime);
		getTransform()->position += getRigidBody()->velocity * (deltaTime);
		velocity += acceleration * (deltaTime * 2);
	}
	else {
		getRigidBody()->velocity.y = 0;
		if (getRigidBody()->velocity.x > 0) 
		{
			if (checker) 
			{
				getRigidBody()->velocity.x = velocity;

				checker = false;		// check if hits the ground
			}
			getRigidBody()->velocity.x -= acceleFric * (deltaTime);
		}
		else 
		{
			getRigidBody()->velocity.x = 0;
		}
		getTransform()->position += getRigidBody()->velocity * (deltaTime);
	}
}

void Target::release()
{
	checker = true;
	getTransform()->position = releasePosition;
	getRigidBody()->velocity = releaseSpeed;
	velocity = 0;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
