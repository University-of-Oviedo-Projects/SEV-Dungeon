#pragma once
#include "Actor.h"
#include "Animation.h"
#include "Player.h"

class Recolectable : public Actor {
public:
    static int totalRecolectados; // Contador global de ítems recolectados
    Recolectable(float x, float y, Game* game);
    Recolectable(string url, float x, float y, Game* game);
    void update();

    // Sobrescribir el método de dibujar
    void draw(float scrollX = 0, float scrollY = 0) override;

private:    
    Animation* animation; // Animación del ítem
};
