#pragma once
#include "Actor.h"

class Projectile : public Actor 
{
public:
	Projectile(string url, float x, float y, Game* game);
	Projectile(string url, float x, float y, Game* game, float direction);
	Projectile(string url, float x, float y, Game* game, float directionX, float directionY);
	void update();
};
