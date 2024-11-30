#pragma once

#include "BaseEnemy.h"
#include "Projectile.h"

class Enemy1 : public BaseEnemy
{
public:
    // Constructor del enemigo básico
    Enemy1(float x, float y, Game* game);

    void update() override;
    void updateMovement() override;

    // Método opcional para disparar
    Projectile* autoShoot();
    
    // Implementación de los métodos para manejar shootsRemaining
    int getShootsRemaining() const;
    void setShootsRemaining(int shoots);

    int shootCadence;
    int shootTime;
	int shootsRemaining;
};
