#include "Recolectable.h"
#include "Animation.h"

Recolectable::Recolectable(float x, float y, Game* game)
    : Actor("res/disparo_jugador.png", x, y, 16, 6, game) {

    // Inicializamos la animación
    animation = new Animation("res/disparo_jugador.png", width,
        height, 16, 6, 1, 1, true, game);
}

Recolectable::Recolectable(string url, float x, float y, Game* game)
    : Actor(url, x, y, 36, 29, game) {
    
    // Inicializamos la animación
    animation = new Animation(url, width, height, 36, 29, 1, 1, true, game);
}

// Actualizamos la animación en el método 'update'
void Recolectable::update() {
    animation->update();
}

// Adaptamos el método 'draw' para usar el desplazamiento en X (scrollX)
void Recolectable::draw(float scrollX, float scrollY) {
    if (animation) {
      // Dibujamos la animación en la posición ajustada por el desplazamiento
      animation->draw(x - scrollX, y - scrollY); // Ajustar tanto X como Y
    }
}
