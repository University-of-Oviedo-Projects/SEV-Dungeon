#pragma once
#include "Actor.h"
#include "Animation.h"

class Box : public Actor {
public:
    Box(float x, float y, Game* game);
    void update();

    // Sobrescribir el método de dibujar
    void draw(float scrollX = 0, float scrollY = 0) override;

    void move(float xMove, float yMove);

    bool isCollidingWithPlayer;

private:
    Animation* animation; // Animación del ítem
};
