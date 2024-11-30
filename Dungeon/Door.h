#pragma once
#include "Actor.h"
#include "Animation.h"
#include <map>
#include <list>
#include "Box.h"

class Door : public Actor {
public:
    enum DoorState {
        DOOR_SHUT,
        DOOR_OPENING,
        DOOR_OPEN,
        DOOR_CLOSING
    };

    Door(float x, float y, Game* game);
    void update();
    DoorState getState();
    bool isPuzzleCompleted(list<Box*> boxes);
    void draw(float scrollX, float scrollY);
    void setState(DoorState newState);

private:
    DoorState state; // Estado actual de la puerta
    Animation* animation;
    std::map<DoorState, Animation*> animations; // Animaciones para cada estado
};
