#include "Enemy1.h"
#include "Projectile.h" 

Enemy1::Enemy1(float x, float y, Game* game)
    : BaseEnemy("res/enemigo.png", x, y, game) {

    orientation = -1;
    state = game->stateMoving; 

    vxIntelligence = 3;
    vyIntelligence = 0;
    vx = vxIntelligence;  
    vy = vyIntelligence; 

    shootTime = 0;
    isFrozen = false;
	timeFrozen = 12000; 
    setShootsRemaining(1);
    shootCadence = (rand() % (150 - 90)) + 90;

    aDying = 
        new Animation("res/enemigo_morir.png", width, height, 280, 40, 6, 8, false, game);
    aMoving = 
        new Animation("res/enemigo_movimiento.png", width, height, 108, 40, 6, 3, true, game); 
    animation = aMoving;  
}

void Enemy1::update() {
    if (isFrozen) {
        timeFrozen -= 1; 
        if (timeFrozen <= 0) { isFrozen = false; }
        return; 
    }

    bool endAnimation = animation->update();
    if (endAnimation) {
        if (state == game->stateDying) { state = game->stateDead; }
    }

    if (state == game->stateMoving) { animation = aMoving; }
    else if (state == game->stateDying) { animation = aDying; }

    if (state != game->stateDying) { updateMovement(); } else { vx = 0; }

    animation->update();
    autoShoot();
}

void Enemy1::updateMovement() {
	if (orientation > 0) {
		vx = vxIntelligence;  // Movimiento hacia la derecha
	}
	else {
		vx = -vxIntelligence;  // Movimiento hacia la izquierda
	}
}

Projectile* Enemy1::autoShoot() {
    shootTime++;  // Incrementa el tiempo de disparo
    if (shootTime > shootCadence) {
        shootTime = 0;  // Reinicia el tiempo de disparo
        float projectileDirection = (orientation > 0) ? 1.0f : -1.0f; 
        float shootX = (orientation > 0) ? x + width / 2 : x - width / 2;
        return new Projectile("res/disparo_jugador.png", shootX, y, game, projectileDirection);
    }
    return nullptr; // No dispara
}

int Enemy1::getShootsRemaining() const {
    return shootsRemaining;
}

void Enemy1::setShootsRemaining(int shoots) {
    shootsRemaining = shoots;
}
