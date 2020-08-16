#include "Enemy.h"
#include "Engine.h"
#include "Util.h"

Enemy::Enemy()
{
	patrolPointIndex = 0;

	m_currentHeading = 0.0;
	m_turnRate = 5.0f;
	m_currentDirection = glm::vec2(1.0f, 0.0f);
	m_angleToTarget = 0.0f;

	m_frame = 0;
	m_frameMax = 8;
	m_Sprite = 0;
	m_SpriteMax = 6;
}

Enemy::~Enemy()
{
	{
		Behavior* m_currentBehavior;

		void BuildList() {
			Behavior* idle = new Behavior();
			Behavior* attack = new Behavior();
			Behavior* seek = new Behavior();
			Behavior* flee = new Behavior();
			Behavior* pathfind = new Behavior();
			Behavior* ranged = new Behavior();
			Behavior* closeCombat = new Behavior();
			Behavior* moveBehindCover = new Behavior();
			Behavior* waitBehindCover = new Behavior();

			
			
			{
				idle->next.push_back(seek);
				idle->next.push_back(flee);
				idle->next.push_back(pathfind);
				idle->next.push_back(moveBehindCover);
				idle->next.push_back(waitBehindcover);
			}
			
			{ 

				attack->next.push_back(ranged);
				attack->next.push_back(closeCombat);
				attack->next.push_back(seek);
				attack->next.push_back(flee);
				attack->next.push_back(waitBehindcover);
				attack->next.push_back(moveBehindCover);
				attack->next.push_back(pathfind);
			}


			{

				seek->next.push_back(ranged);
				seek->next.push_back(flee);
				seek->next.push_back(waitBehindcover);
				seek->next.push_back(moveBehindCover);
				seek->next.push_back(pathfind);

			}

			{

				flee->next.push_back(waitBehindcover);
				flee->next.push_back(moveBehindCover);
				flee->next.push_back(pathfind);

			}

			idle = nullptr;
			attack = nullptr;
			enemy = nullptr;
			seek = nullptr;
			pathfind = nullptr;
			ranged = nullptr;
			closeCombat = nullptr;
			moveBehindCover = nullptr; 
			waitBehindCover = nullptr;
		}
		void setBehavior(Behavior * next);
		void update() { m_currentBehavior->Update() }
	};
}

void Enemy::Draw()
{
	int xComponent = GetPosition().x;
	int yComponent = GetPosition().y;

	TextureManager::Instance()->modifiedDraw("enemy", xComponent, yComponent, 6, imageOffset, Engine::Instance().GetRenderer(), m_currentHeading, 255, true);
}

void Enemy::Update()
{
	m_animate();
	m_computeTargetDirection();
	move();
	check_Destination();
}

void Enemy::m_animate()
{
	if (m_frame == m_frameMax)
	{
		m_frame = 0;
		m_Sprite++;
		if (m_Sprite == m_SpriteMax)
		{
			m_Sprite = 0;
		}
		imageOffset = (GetWidth() / m_SpriteMax) * m_Sprite;
	}
	m_frame++;

}

void Enemy::move()
{
	m_angleToTarget = Util::SignedAngle(getCurrentDirection(), m_targetDirection);
	if (m_angleToTarget > 0.0f)
	{
		turnRight();
	}
	if (m_angleToTarget < 0.0f)
	{
		turnLeft();
	}
	SetVelocity(getCurrentDirection());

	if (abs(m_angleToTarget) < 91.0f)
	{
		moveTowardPoint();
	}
}

void Enemy::moveTowardPoint()
{

	std::cout << patrolPointIndex << std::endl;
	std::cout << "Size: " << m_PatrolPoints.size() << std::endl;


	if (patrolPointIndex <= m_PatrolPoints.size())
	{
		setTargetPosition(m_PatrolPoints[patrolPointIndex]);
		float distance = Util::Distance(GetPosition(), getTargetPosition());
		float radius = 6.0f;
		if (distance < radius)
		{
			SetVelocity(Util::Normalize(getTargetPosition() - GetPosition()) * getMaxSpeed() * (distance / radius));
		}
		else
		{
			//setVelocity(Util::normalize(getTargetPosition() - getPosition()));
		}
		glm::vec2 newPosition = GetPosition() + GetVelocity() * getMaxSpeed();
		SetPosition(newPosition);
	}
}

void Enemy::turnRight()
{
	float angleDiff = abs(abs(m_angleToTarget) - m_turnRate);
	m_currentHeading += (angleDiff < m_turnRate) ? angleDiff : m_turnRate;
	if (m_currentHeading > 360)
	{
		m_currentHeading -= 360.0f;
	}
	m_changeDirection();
	m_computeTargetDirection();
}

void Enemy::turnLeft()
{
	float angleDiff = abs(abs(m_angleToTarget) - m_turnRate);
	m_currentHeading -= (angleDiff < m_turnRate) ? angleDiff : m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}
	m_changeDirection();
	m_computeTargetDirection();
}

void Enemy::turnUP()
{
	float angleDiff = abs(abs(m_angleToTarget) - m_turnRate);
	m_currentHeading += (angleDiff < m_turnRate) ? angleDiff : m_turnRate;
	if (m_currentHeading > 360)
	{
		m_currentHeading -= 360.0f;
	}
	m_changeDirection();
	m_computeTargetDirection();
}

void Enemy::turnDOWN()
{
	float angleDiff = abs(abs(m_angleToTarget) - m_turnRate);
	m_currentHeading -= (angleDiff < m_turnRate) ? angleDiff : m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}
	m_changeDirection();
	m_computeTargetDirection();
}



void Enemy::check_Destination()
{
	if (patrolPointIndex < m_PatrolPoints.size() - 1)
	{
		if (GetPosition() == getTargetPosition())
		{
			patrolPointIndex++;
		}
	}
	else
	{
		patrolPointIndex = 0;
	}
}

void Enemy::m_changeDirection()
{
	float x = cos(m_currentHeading * Util::Deg2Rad);
	float y = sin(m_currentHeading * Util::Deg2Rad);

	m_currentDirection = glm::vec2(x, y);
	glm::vec2 size = TextureManager::Instance()->getTextureSize("ship");
}

void Enemy::m_computeTargetDirection()
{
	glm::vec2 steeringVelocity = getTargetPosition() - GetPosition();
	m_targetDirection = Util::Normalize(steeringVelocity);
}

void Enemy::Clean()
{
}

float Enemy::getMaxSpeed()
{
	return m_maxSpeed;
}

glm::vec2 Enemy::getTargetPosition()
{
	return m_targetPosition;
}

glm::vec2 Enemy::getCurrentDirection()
{
	return m_currentDirection;
}

void Enemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Enemy::setTargetPosition(glm::vec2 newTargetPosition)
{
	m_targetPosition = newTargetPosition;
}

void Enemy::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

EnemyState Enemy::getState() {
	return m_State;
}

void Enemy::setState(EnemyState m_State) {
	this->m_State = m_State;
}

void Enemy::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Enemy::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-idle-3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-run-3"));

	setAnimation(runAnimation);
}

class Behavior
{
	Enemy* currentEnemy;
	virtual void Update() = 0;
	std::vector<Behavior*> m_next;
}; 

class Idle : public Behavior
{
	bool los;
	void Update() {
		if (los) {
			currentEnemy->setBehavior(m_next);
		}
	}
};

class Seek : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
};

class attack : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
};

class ranged : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float; 
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
}; 

class closeCombat : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
};

class moveBehindCover : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
}; 

class waitBehindCover  : public Behavior
{
	float distance;
	float DistanceToPlayer() {
		
		return as float
	}

	void Update() {
		distance = DistanceToPlayer();
		if (distance < 100.0f)
		{
			currentEnemy->setBehavior(m_next);
		}
		
	}
}; 

