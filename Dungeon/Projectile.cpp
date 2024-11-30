#include "Projectile.h"

Projectile::Projectile(string url, float x, float y, Game* game) :
	Actor(url, x, y, 20, 20, game) {
	vx = 2;
	vy = 0;
}

Projectile::Projectile(string url, float x, float y, Game* game, float direction) :
	Actor(url, x, y, 20, 20, game) {
	vx = 5 * direction; // Establecer la velocidad en el eje X en función de la dirección
	vy = 0; // No hay movimiento vertical
}

Projectile::Projectile(string url, float x, float y, Game* game, 
		float directionX, float directionY) :
	Actor(url, x, y, 20, 20, game) {
	vx = 5 * directionX; // Establecer la velocidad en el eje X en función de la dirección
	vy = 5 * directionY; // No hay movimiento vertical
}

void Projectile::update() {
	x = x + vx;
	y = y + vy;
}