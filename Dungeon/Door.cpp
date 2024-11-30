#include "Door.h"
#include "Game.h"
#include <list>
#include "Box.h"

Door::Door(float x, float y, Game* game)
    : Actor("res/door_shut.png", x, y, 62, 74, game), state(DOOR_SHUT) {
    // Animaciones para cada estado
    animations[DOOR_SHUT] = 
        new Animation("res/door_shut.png", width, height, 62, 74, 1, 1, true, game);
    animations[DOOR_OPEN] = 
        new Animation("res/door_open.png", width, height, 62, 74, 1, 1, true, game);

    // Comenzamos con la animación de puerta cerrada
    animation = animations[DOOR_SHUT];
}

void Door::update() {
    // Actualizamos la animación actual
    if (animation) {
        animation->update();
    }
}

Door::DoorState Door::getState() {
    return state;
}

bool Door::isPuzzleCompleted(list<Box*> boxes) {
    bool leftBoxFound = false;
    bool rightBoxFound = false;
    float proximityThreshold = 60.0f;

    for (Box* box : boxes) {
        if (box->x < this->x && (this->x - box->x) <= proximityThreshold) {
            leftBoxFound = true;
        }
        else if (box->x > this->x && (box->x - this->x) <= proximityThreshold) {
            rightBoxFound = true;
        }

		// Si se encuentran las dos cajas en las proximidades, activamos la puerta
        if (leftBoxFound && rightBoxFound) {
            if (state == DOOR_SHUT) {
                setState(DOOR_OPEN);  
            }
			return true;  // Puzzle completado
        }
    }

    return false;
}

void Door::draw(float scrollX, float scrollY) {
    if (animation) {
        animation->draw(x - scrollX, y - scrollY);
    }
}

// Cambia el estado de la puerta y la animación asociada
void Door::setState(DoorState newState) {
    if (state != newState) {
        state = newState;
        animation = animations[state];
        animation->update(); 
    }
}
