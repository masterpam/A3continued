#include "Obstacle.h"

#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	const auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(OBSTACLE);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("obstacle", x, y, 0, 255, true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
} 

void Obstacle::setAnimationState(const ObstacleAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Obstacle::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Obstacle-run-3"));

	setAnimation(runAnimation);
}
