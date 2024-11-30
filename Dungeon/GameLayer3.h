#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "BaseEnemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

#include "Pad.h"
#include "Recolectable.h"
#include "Box.h"
#include "Tile.h"
#include "Door.h"

class GameLayer3 : public Layer
{
public:
	GameLayer3(Game* game);
	void init() override;
	void processControls() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	void update() override;
	void updateGameObjects();
	bool checkLevelCompletion();
	void handleEnemyProjectiles();
	void handleEnemySeeingPlayer();
	void handlePlayerTileCollisions();
	void handleEnemyTileCollisions();
	void resolveEnemy1TileCollision(Enemy1* enemy1, Tile* tile);
	void resolveEnemy2TileCollision(Enemy2* enemy2, Tile* tile);
	void handlePlayerCollisions();
	void resolvePlayerTileCollision(Tile* tile);
	void handleCollectables();
	void handleProjectileCollisions();
	void handleBoxCollisions();

	Space* space;
	Door* door;
	float scrollX;
	float scrollY;
	int mapWidth;
	int mapHeight;

	int newEnemyTime = 0;
	Player* player;
	list<BaseEnemy*> enemies;
	list<Projectile*> projectiles;
	list<Recolectable*> municiones;
	list<Recolectable*> curaciones;
	list<Projectile*> enemyProjectiles;
	list<Projectile*> projectilesFreeze;
	list<Box*> boxes;
	list<Tile*> tiles;

	Audio* audioBackground;
	Background* background;
	Actor* backgroundMunicionCogida;
	int municionCogida;
	Text* textMunicionCogida;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
	bool freezingAttack = false;
	int enemigosEliminados;
	Text* textEnemigosEliminados;
	Actor* backgroundEnemigosEliminados;
	int vidasJugador;
	Text* textVidasJugador;
	Actor* backgroundVidasJugador;
	int ataquesHielo;
	Text* textAtaquesHielo;
	Actor* backgroundAtaquesHielo;
	Actor* backgroundEscudo;

	Pad* pad;
	Actor* buttonFreezing;
	Actor* buttonShoot;
	Actor* buttonShield;
	SDL_GameController* gamePad;
	Actor* message;
	bool pause;
	bool controlContinue = false;

	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD

};

