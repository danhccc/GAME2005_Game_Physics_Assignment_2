#include "Target.h"
#include "TextureManager.h"



Target::Target()
{
	TextureManager::Instance()->load("../Assets/sprites/thermalDetonator.png","bomb");

	const auto size = TextureManager::Instance()->getTextureSize("bomb");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(140.0f, 570.0f);
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
	TextureManager::Instance()->draw("bomb", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::doThrow()
{
	getTransform()->position = throwPosition;
	getRigidBody()->velocity = throwSpeed;
}

void Target::m_move()
{
	float deltaTime = 1.0f / 30.0f;
	glm::vec2 gravity = glm::vec2(0, 9.8f);

	getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;

	if (!isGravityEnabled) getRigidBody()->velocity.y = 0;

	getTransform()->position += getRigidBody()->velocity * deltaTime ;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
