#include "Box.h"
#include "Animation.h"
#include "Player.h"

Box::Box(float x, float y, Game* game)
    : Actor("res/caja_madera.png", x, y, 25, 25, game) {

    // Inicializamos la animación
    animation = new Animation("res/caja_madera.png", width,
        height, 40, 32, 6, 8, true, game);

    isCollidingWithPlayer = false;
}

// Actualizamos la animación en el método 'update'
void Box::update() {
    animation->update();
}

// Adaptamos el método 'draw' para usar el desplazamiento en X (scrollX)
void Box::draw(float scrollX, float scrollY) {
    if (animation) {
        animation->draw(x - scrollX, y - scrollY); // Ajustar tanto X como Y
    }
}

// Método que permite el movimiento en X solo si hay colisión con el jugador
void Box::move(float xMove, float yMove) {
    if (isCollidingWithPlayer) {
        x += xMove + 1;
    }
}