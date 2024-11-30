#pragma once

#include "BaseEnemy.h"
#include "Player.h"

class Enemy2 : public BaseEnemy
{
public:
    // Constructor del enemigo básico
    Enemy2(float x, float y, Game* game);

    void update() override;
    void updateMovement() override;

	float lerp(float current, float target, float alpha);
	bool canSeePlayer();
	float calculateDistanceToPlayer();

	int orientation; // Orientación
	Player* player;

private:
	Animation* aIdle;
	Animation* aAttackingDown; 
	Animation* aAttackingUp;
	Animation* aAttackingLeft;
	Animation* aAttackingRight;
	Animation* aMovingRight;
	Animation* aMovingUp;
	Animation* aMovingDown;
};
