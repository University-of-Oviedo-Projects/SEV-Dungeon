#include "Enemy2.h"
#include "GameLayer.h"

Enemy2::Enemy2(float x, float y, Game* game)
    : BaseEnemy("res/enemigo.png", x, y, game) {

    orientation = -1;
	state = game->stateIdle;
    vxIntelligence = -3;
    vyIntelligence = 0;
    vx = vxIntelligence;
    vy = vyIntelligence;
    isFrozen = false;
    timeFrozen = 12000;

	// Animacion que se muestra cuando el enemigo está quieto
    aIdle = new Animation("res/animacion_rey_quieto.png", width, height,
        80, 44, 6, 2, true, game); 

	// Animacion que se muestra cuando el enemigo muere
    aDying = new Animation("res/enemigo_morir.png", width, height,
        280, 40, 6, 8, false, game); 

    // Animaciones de movimiento
    aMoving = new Animation("res/animacion_rey_izquierda.png", width, height,
        80, 44, 6, 2, true, game);
    aMovingRight = new Animation("res/animacion_rey_derecha.png", width, height,
        80, 44, 6, 2, true, game);
    aMovingUp = new Animation("res/animacion_rey_arriba.png", width, height,
        80, 44, 6, 2, true, game);
    aMovingDown = new Animation("res/animacion_rey_abajo.png", width, height,
        80, 44, 6, 2, true, game);

    // Animaciones de ataque
    aAttackingRight = new Animation("res/animacion_rey_ataque_derecha.png", width, height,
        240, 44, 8, 6, false, game);
    aAttackingLeft = new Animation("res/animacion_rey_ataque_izquierda.png", width, height,
        240, 44, 8, 6, false, game);
    aAttackingUp = new Animation("res/animacion_rey_ataque_arriba.png", width, height,
        240, 44, 8, 6, false, game);
    aAttackingDown = new Animation("res/animacion_rey_ataque_abajo.png", width, height,
        240, 44, 8, 6, false, game);

    // Animación inicial
    animation = aIdle;  
}

// Método que actualiza el estado del enemigo
void Enemy2::update() {
    if (isFrozen) {
        timeFrozen -= 1; 
        if (timeFrozen <= 0) {isFrozen = false;}
        return; 
    }

    bool endAnimation = animation->update();
    if (endAnimation) {
        if (state == game->stateDying) { state = game->stateDead;}
    }

    if (state == game->stateMoving) { animation = aMoving;}
    else if (state == game->stateDying) { animation = aDying; }

    if (state != game->stateDying && player != nullptr) { updateMovement(); } 
    else { vx = 0; }
    animation->update();
}

// Método que gestiona el movimiento del enemigo
void Enemy2::updateMovement() {
    float dx = player->x - x;
    float dy = player->y - y;
    float magnitude = sqrt(dx * dx + dy * dy);

    if (magnitude > 0) {
        float targetVx = (dx / magnitude) * 3;
        float targetVy = (dy / magnitude) * 3;
        float smoothFactor = 0.1f;  
        vxIntelligence = lerp(vxIntelligence, targetVx, smoothFactor);
        vyIntelligence = lerp(vyIntelligence, targetVy, smoothFactor);

    } else {vxIntelligence = 0; vyIntelligence = 0;}

	if (vxIntelligence > 0) {animation = aMovingRight;}
	else if (vxIntelligence < 0) {animation = aMoving;}
	else if (vyIntelligence > 0) {animation = aMovingDown;}
	else if (vyIntelligence < 0) {animation = aMovingUp;}
	vx = vxIntelligence; vy = vyIntelligence;
}

float Enemy2::lerp(float current, float target, float alpha) {
    return current + alpha * (target - current);
}

bool Enemy2::canSeePlayer() {
    float distanceToPlayer = calculateDistanceToPlayer();
    const float SEE_DISTANCE_TRESHOLD = 200.0f;

    if (distanceToPlayer < SEE_DISTANCE_TRESHOLD) { return true; }
	return false;  
}

float Enemy2::calculateDistanceToPlayer() {
    float dx = player->x - x;
    float dy = player->y - y;

	// Eucledian distance
    return sqrt(dx * dx + dy * dy);
}
