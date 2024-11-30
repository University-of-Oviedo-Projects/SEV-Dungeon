#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Projectile.h" // incluir proyectil
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void loseLife();
	Projectile* attack();
	Projectile* freezingAttack();
	int lifes = 3;
	int invulnerableTime = 0;
	int orientation;
	int state;
	
	Audio* audioShoot;
	int shootCadence = 30;
	bool shieldActive;
	int numberOfShots;
	int numberOfFreezingAttacks;
	int timeToNextShot = 0;

	Animation* aIdleRight; // Animación cuando el enemigo está quieto mirando a la derecha
	Animation* aIdleLeft;  // Animación cuando el enemigo está quieto mirando a la izquierda
	Animation* aIdleUp;    // Animación cuando el enemigo está quieto mirando arriba
	Animation* aIdleDown;  // Animación cuando el enemigo está quieto mirando abajo
	Animation* aMovingRight; // Animación cuando el enemigo está moviéndose mirando a la derecha
	Animation* aMovingLeft;  // Animación cuando el enemigo está moviéndose mirando a la izquierda
	Animation* aMovingUp;    // Animación cuando el enemigo está moviéndose mirando arriba
	Animation* aMovingDown;  // Animación cuando el enemigo está moviéndose mirando abajo
	Animation* aAttackingRight; // Animación cuando el enemigo está atacando mirando a la derecha
	Animation* aAttackingLeft;  // Animación cuando el enemigo está atacando mirando a la izquierda
	Animation* aAttackingUp;    // Animación cuando el enemigo está atacando mirando arriba
	Animation* aAttackingDown;  // Animación cuando el enemigo está atacando mirando abajo
	Animation* aDying;     // Animación cuando el enemigo está muriendo
	Animation* animation;  // Referencia a la animación actual
};

