#pragma once

#include "Actor.h"
#include "Animation.h"

class BaseEnemy : public Actor
{
public:
    // Constructor que toma los parámetros necesarios para inicializar un Actor
    BaseEnemy(const std::string& imagePath, float x, float y, Game* game);

    // Sobrescribir el método de dibujar
    void draw(float scrollX = 0, float scrollY = 0) override;

    // Método abstracto para actualizar el comportamiento del enemigo
    virtual void update() = 0;

    // Método abstracto para actualizar el movimiento
    virtual void updateMovement() = 0;

    // Método que se llama cuando el enemigo recibe impacto (morir, etc.)
    void impacted();   

    float vxIntelligence;  // Velocidad o inteligencia del movimiento
	float vyIntelligence;  // Velocidad vertical

    int state;             // Estado del enemigo (moviendo, muriendo, etc.)
    int orientation;       // Orientación
	bool isFrozen;		 // Indica si el enemigo está congelado
	float timeFrozen;    // Tiempo que el enemigo está congelado

    Animation* aDying;     // Animación cuando el enemigo está muriendo
    Animation* aMoving;    // Animación cuando el enemigo se está moviendo
    Animation* animation;  // Referencia a la animación actua
};
