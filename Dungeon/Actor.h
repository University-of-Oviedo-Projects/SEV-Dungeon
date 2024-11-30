#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	bool isOverlapWithProposed(Actor* other, float proposedX, float proposedY);
	~Actor();
	virtual void draw(float scrollX = 0, float scrollY = 0);
	bool isOverlap(Actor* actor);
	bool isFrontOverlap(Actor* actor, int orientation);
	bool isInRender(float scrollX = 0);

	bool containsPoint(int pointX, int pointY); // contiene punto
	bool collisionDown;
	bool clicked; // Está pulsado

	SDL_Texture* texture;
	int x;
	int y;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	Game* game; // referencia al juego

	bool outRight;
	bool outLeft;
};

