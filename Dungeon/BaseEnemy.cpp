#include "BaseEnemy.h"
#include "Game.h"

// Constructor que inicializa las propiedades comunes a todos los enemigos
BaseEnemy::BaseEnemy(const std::string& imagePath, float x, float y, Game* game)
    : Actor(imagePath, x, y, 36, 40, game) { 

    vxIntelligence = 0.5f;  // Inicializa la velocidad horizontal (ajustable según el enemigo)
	vyIntelligence = 0;     // Inicializa la velocidad vertical (ajustable según el enemigo)

    state = 0;              // Estado inicial: 0 para moviéndose
	orientation = 0;        // Orientación inicial

	aMoving = nullptr;
	aDying = nullptr;
	animation = nullptr; 
    isFrozen = false;
    timeFrozen = 0;
}

// Método común para gestionar el impacto recibido por el enemigo
void BaseEnemy::impacted() {
    if (state != game->stateDying) {
        state = game->stateDying;
        animation = aDying;
    }
}

// Método para dibujar al enemigo en pantalla
void BaseEnemy::draw(float scrollX, float scrollY) {
    if (animation != nullptr) {
        animation->draw(x - scrollX, y - scrollY);
    }
}

// Método abstracto para actualizar el estado del enemigo 
void BaseEnemy::update() {
    // Este método es abstracto, y será implementado en las subclases.
}

// Método abstracto para actualizar el movimiento del enemigo
void BaseEnemy::updateMovement() {
    // Este método es abstracto, y será implementado en las subclases.
}