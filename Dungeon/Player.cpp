#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) {

	lifes = 3; 
	numberOfShots = 10;
    invulnerableTime = 5;
	vx = 0; vy = 0;
    shieldActive = false;
    timeToNextShot = 0;
    state = game->stateMoving;
    orientation = game->orientationRight;
	audioShoot = new Audio("res/efecto_disparo.wav", false);

	// Animaciones
	aIdleRight = new Animation("res/jugador_idle_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	aIdleLeft = new Animation("res/jugador_idle_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aIdleUp = new Animation("res/jugador_idle_izquierda.png", width, height,
        320, 40, 6, 8, true, game);
	aIdleDown = new Animation("res/jugador_idle_izquierda.png", width, height,
        320, 40, 6, 8, true, game);

	// Animaciones de movimiento
	aMovingRight = new Animation("res/jugador_corriendo_derecha.png", width, height,
		320, 40, 6, 8, true, game);
	aMovingLeft = new Animation("res/jugador_corriendo_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aMovingUp = new Animation("res/jugador_corriendo_izquierda.png", width, height,
		320, 40, 6, 8, true, game);
	aMovingDown = new Animation("res/jugador_corriendo_izquierda.png", width, height,
		320, 40, 6, 8, true, game);

	// Animaciones de ataques
	aAttackingRight = new Animation("res/jugador_disparando_derecha.png",
		width, height, 160, 40, 6, 4, false, game);
	aAttackingLeft = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);
	aAttackingUp = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);
	aAttackingDown = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);

	// Animación de morir
	aDying = new Animation("res/Protagonista/animacion_rey_muerto.png", width, height,
		320, 40, 6, 8, true, game);

	// Animación inicial
	animation = aIdleRight;
}

void Player::update() {
    if (invulnerableTime > 0) {invulnerableTime--;}
    bool endAnimation = animation->update();
    if (endAnimation && state == game->stateShooting) {state = game->stateMoving;}

    if (vx == 0 && vy == 0) {state = game->stateIdle;}
	else {state = game->stateMoving;}

    if (vx > 0) {orientation = game->orientationRight; }
    else if (vx < 0) {orientation = game->orientationLeft;}

    if (vy > 0) {orientation = game->orientationDown;}
    else if (vy < 0) {orientation = game->orientationUp;}

	if (state == game->stateIdle) {
		if (orientation == game->orientationRight) {animation = aIdleRight;}
		else if (orientation == game->orientationLeft) {animation = aIdleLeft;}
		else if (orientation == game->orientationUp) {animation = aIdleUp;}
		else if (orientation == game->orientationDown) {animation = aIdleDown;}

	} else if (state == game->stateShooting) {
        if (orientation == game->orientationRight) {animation = aAttackingRight;}
        else if (orientation == game->orientationLeft) {animation = aAttackingLeft;}
		else if (orientation == game->orientationUp) { animation = aAttackingUp;}
        else if (orientation == game->orientationDown) {animation = aAttackingDown;}

    } else if (state == game->stateMoving) {
        if (vx != 0 || vy != 0) {  
            if (orientation == game->orientationRight) { animation = aMovingRight; }
            else if (orientation == game->orientationLeft) { animation = aMovingLeft; }
            else if (orientation == game->orientationUp) { animation = aMovingUp; }
            else if (orientation == game->orientationDown) { animation = aMovingDown; }
        }
        else {  
            if (orientation == game->orientationRight) { animation = aIdleRight; }
            else if (orientation == game->orientationLeft) { animation = aIdleLeft; }
            else if (orientation == game->orientationUp) { animation = aIdleUp; }
            else if (orientation == game->orientationDown) { animation = aIdleDown;}
        }
    }

    if (timeToNextShot > 0) { timeToNextShot--; }
    x += vx; y += vy; 
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

void Player::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

Projectile* Player::attack() {
    if (timeToNextShot > 0) { return nullptr; }
	if (numberOfShots <= 0) { return nullptr; }
    timeToNextShot = 50; numberOfShots--;

    float projectileDirectionX = 0.0f; // Dirección en el eje X
    float projectileDirectionY = 0.0f; // Dirección en el eje Y

    if (orientation == game->orientationRight) {
        projectileDirectionX = 1.0f;  // Disparar hacia la derecha
    }
    else if (orientation == game->orientationLeft) {
        projectileDirectionX = -1.0f;  // Disparar hacia la izquierda
    }
    else if (orientation == game->orientationDown) {
        projectileDirectionY = 1.0f;  // Disparar hacia abajo
    }
    else if (orientation == game->orientationUp) {
        projectileDirectionY = -1.0f;  // Disparar hacia arriba
    }

    float shootX = (orientation > 0) ? x + width / 2 : x - width / 2;
    return new Projectile("res/disparo_jugador.png", shootX, y,
            game, projectileDirectionX, projectileDirectionY);
}

Projectile* Player::freezingAttack() {
	if (numberOfFreezingAttacks <= 0) { return nullptr; }
	numberOfFreezingAttacks--;

    float projectileDirectionX = 0.0f; // Dirección en el eje X
    float projectileDirectionY = 0.0f; // Dirección en el eje Y

    if (orientation == game->orientationRight) {
        projectileDirectionX = 1.0f;  // Disparar hacia la derecha
    }
    else if (orientation == game->orientationLeft) {
        projectileDirectionX = -1.0f;  // Disparar hacia la izquierda
    }
    else if (orientation == game->orientationDown) {
        projectileDirectionY = 1.0f;  // Disparar hacia abajo
    }
    else if (orientation == game->orientationUp) {
        projectileDirectionY = -1.0f;  // Disparar hacia arriba
    }

    // Crear un proyectil ligeramente delante del enemigo
    float shootX = (orientation > 0) ? x + width / 2 : x - width / 2;
    return new Projectile("res/disparo_hielo.png", shootX, y,
        game, projectileDirectionX, projectileDirectionY);
}


void Player::loseLife() {
    if (state != game->stateDying && state != game->stateDead) {state = game->stateDying;}
}
