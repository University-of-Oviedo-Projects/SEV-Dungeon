#include "Background.h"

Background::Background(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {

}

Background::Background(string filename, float x, float y, float vx, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {

	this->vx = vx;
	if (vx != 0) {
		backgroundAux = new Background(filename, x + WIDTH, y, game);
	}
}

void Background::update() {
	if (vx != 0) {
		x = x + vx;

		// se salio por la izquierda
		if (x + width / 2 < 0) {
			// vuelve a aparecer por la derecha
			x = WIDTH + width / 2;
		}
		// se salio por la derecha
		if (x - width / 2 > WIDTH) {
			// vuelve por la izquierda
			x = 0 - width / 2;
		}
	}

	if (vy != 0) {
		y += vy;

		// Salida por arriba
		if (y + height / 2 < 0) {
			y = HEIGHT + height / 2; // Vuelve a aparecer por abajo
		}
		// Salida por abajo
		if (y - height / 2 > HEIGHT) {
			y = 0 - height / 2; // Vuelve por arriba
		}
	}
}

void Background::draw(float scrollX, float scrollY) {
	// Llamar al método draw del padre (Actor)
	Actor::draw(scrollX, scrollY); // Pasar scrollX y scrollY

	if (backgroundAux != NULL) {
		// Zona sin cubrir por la izquierda
		if (x - width / 2 > 0) {
			// Pintar aux por la izquierda
			backgroundAux->x = x - width; // Ajustar la posición X de backgroundAux
			backgroundAux->y = y;         // Mantener la misma posición Y
		}
		// Zona sin cubrir por la derecha
		if (x + width / 2 < WIDTH) {
			// Pintar aux por la derecha
			backgroundAux->x = x + width; // Ajustar la posición X de backgroundAux
			backgroundAux->y = y;          // Mantener la misma posición Y
		}
		backgroundAux->draw(scrollX, scrollY); // Llamar a draw con scrollX y scrollY
	}
}


