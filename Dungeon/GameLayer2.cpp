#include "GameLayer2.h"
#include <iostream> 
using namespace std;
#include <random>
#include <chrono>
#include <thread>
#include "Door.h"

GameLayer2::GameLayer2(Game* game)
	: Layer(game) {
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	gamePad = SDL_GameControllerOpen(0);
	init();
}


void GameLayer2::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);

	float shootX = WIDTH * 0.75 + 66; float shootY = HEIGHT * 0.83;
	float shieldX = shootX; float shieldY = shootY - 105;
	float freezingX = shootX - 105; float freezingY = shootY;

	// Crear los botones con las nuevas posiciones
	buttonShoot = new Actor("res/boton_disparo.png", shootX, shootY, 100, 100, game);
	buttonShield = new Actor("res/boton_escudo.png", shieldX, shieldY, 100, 100, game);
	buttonFreezing = new Actor("res/boton_hielo.png", freezingX, freezingY, 100, 100, game);

	space = new Space(1);
	scrollX = 0; scrollY = 0;
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	// Margen y espaciado para los elementos en la esquina superior derecha
	float marginX = WIDTH - 10; // Margen derecho (alinea al lado derecho de la pantalla)
	float marginY = 10;        // Margen superior
	float spacing = 70;        // Espaciado entre cada par texto-foto

	ataquesHielo = 0;
	vidasJugador = 2;
	municionCogida = 5; // Son los disparos del jugador
	enemigosEliminados = 0;

	// ESCUDO
	backgroundEscudo = new Actor("res/shield.png",
		marginX, marginY, 24, 24, game);
	marginX -= spacing;

	// DISPAROS DE HIELO
	textAtaquesHielo = new Text(to_string(ataquesHielo), marginX - 30, marginY, game);
	backgroundAtaquesHielo = new Actor("res/disparo_hielo - copia.png",
		marginX, marginY, 24, 24, game);
	marginX -= spacing;

	// municiones
	textMunicionCogida = new Text(to_string(municionCogida),
		marginX - 30, marginY, game);
	backgroundMunicionCogida = new Actor("res/coinGold.png",
		marginX, marginY, 24, 24, game);
	marginX -= spacing;

	// ENEMIGOS MATADOS (POINTS)
	textEnemigosEliminados = new Text(to_string(enemigosEliminados),
		marginX - 30, marginY, game);
	backgroundEnemigosEliminados = new Actor("res/icono_puntos.png",
		marginX, marginY, 24, 24, game);
	marginX -= spacing;

	// VIDAS DEL JUGADOR
	textVidasJugador = new Text(to_string(vidasJugador),
		marginX - 30, marginY, game);
	backgroundVidasJugador = new Actor("res/corazon - copia.png",
		marginX, marginY, 28, 23, game);

	// FONDO DEL JUEGO
	background = new Background("res/cavernous.png",
		WIDTH * 0.5, HEIGHT * 0.5, -1, game);

	tiles.clear(); 	enemies.clear(); 	projectiles.clear(); 	projectilesFreeze.clear();
	municiones.clear(); curaciones.clear();	boxes.clear(); 	enemyProjectiles.clear();
	loadMap("res/1.txt");

	if (!player) {
		cout << "Error: No se encontró el jugador en el mapa." << endl;
		return;
	}

	// Centrar la cámara en el jugador
	scrollX = player->x - WIDTH / 2;
	scrollY = player->y - HEIGHT / 2;
	calculateScroll();
}

void GameLayer2::loadMap(string name) {
	cout << name << endl;

	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			mapHeight = (i + 1) * 32; // Donde 32 es la altura de cada fila de tiles

			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}

	streamFile.close();
}

void GameLayer2::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'P': {  // Puerta
		this->door = new Door(x, y, game);
		space->addDynamicActor(door);
		break;
	}
	case 'E': {
		static std::random_device rd;  // Para obtener una semilla
		static std::mt19937 gen(rd());  // Mersenne Twister
		std::uniform_int_distribution<> distr(0, 1); // Distribución uniforme de 0 a 1

		int randomEnemyType = distr(gen);  // Genera 0 o 1

		BaseEnemy* enemy = nullptr;

		if (randomEnemyType == 0) {
			enemy = new Enemy1(x, y, game);
		}
		else {
			enemy = new Enemy2(x, y, game);
		}

		// Modificación para empezar a contar desde el suelo
		enemy->y = enemy->y - enemy->height / 1.25;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'R': {
		Recolectable* municion = new Recolectable(x, y, game);
		municion->y = municion->y - municion->height / 2;
		municiones.push_back(municion);
		space->addDynamicActor(municion);
		break;
	}
	case 'C': {
		Recolectable* curacion = new Recolectable("res/corazon.png", x, y, game);
		curacion->y = curacion->y - curacion->height / 2;
		curaciones.push_back(curacion);
		space->addDynamicActor(curacion);
		break;
	}
	case 'Q': {
		Box* box = new Box(x, y, game);
		box->y = box->y - box->height / 2;
		boxes.push_back(box);
		space->addDynamicActor(box);
		break;
	}
	case '1': {
		player = new Player(x, y, game);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);

		for (auto const& enemy : enemies) {
			if (auto enemy2 = dynamic_cast<Enemy2*>(enemy)) {
				enemy2->player = player;
			}
		}
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/dungeon-wall.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}

void GameLayer2::processControls() {
	// Obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "Error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}

		// Cambio automático de input
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
		}

		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
			if (event.key.keysym.sym == SDLK_F1) { game->scale(); }
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}

		// Procesar inputs
		if (game->input == game->inputGamePad) {
			gamePadToControls(event);
		}
		else if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		else if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}

	// Procesar controles
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	if (freezingAttack && ataquesHielo > 0) {
		Projectile* newProjectile = player->freezingAttack();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectilesFreeze.push_back(newProjectile);
			freezingAttack = false;
			ataquesHielo--;
			textAtaquesHielo->content = to_string(ataquesHielo);
		}

	}
	else if (controlShoot) {
		Projectile* newProjectile = player->attack();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
			controlShoot = false;
			municionCogida--;
			textMunicionCogida->content = to_string(municionCogida);
		}
	}

	// Movimiento en eje X e Y
	player->moveX(controlMoveX);
	player->moveY(controlMoveY);
}

void GameLayer2::mouseToControls(SDL_Event event) {
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;

		// Si el ratón está dentro de la región del pad, se actualiza el control de movimiento
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;

			// Mapea el movimiento a -1 o 1 dependiendo de la flecha presionada en el pad
			controlMoveX = pad->getOrientationX(motionX);
			controlMoveY = pad->getOrientationY(motionY);

			// Control de las flechas (4 direcciones)
			if (controlMoveX > 0) {
				controlMoveX = 1;  // Mueve a la derecha
			}
			else if (controlMoveX < 0) {
				controlMoveX = -1; // Mueve a la izquierda
			}
			else {
				controlMoveX = 0;  // No hay movimiento horizontal
			}

			if (controlMoveY > 0) {
				controlMoveY = 1;  // Mueve hacia abajo
			}
			else if (controlMoveY < 0) {
				controlMoveY = -1; // Mueve hacia arriba
			}
			else {
				controlMoveY = 0;  // No hay movimiento vertical
			}
		}

		// Si el ratón está sobre el botón de disparo, se activa el disparo
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}

		// Si el ratón está sobre el botón de escudo
		if (buttonShield->containsPoint(motionX, motionY)) {
			player->shieldActive = true; // Activar escudo
		}

		// Si el ratón está sobre el botón de congelación
		if (buttonFreezing->containsPoint(motionX, motionY)) {
			freezingAttack = true; // Activar ataque de congelación
		}

		// Si el ratón está sobre el botón de disparo
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
	}

	// Si se mueve el ratón
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			// Actualiza las direcciones de movimiento
			controlMoveX = pad->getOrientationX(motionX);
			controlMoveY = pad->getOrientationY(motionY);

			// Mapea el movimiento a -1 o 1 dependiendo de la dirección
			if (controlMoveX > 0) {
				controlMoveX = 1;
			}
			else if (controlMoveX < 0) {
				controlMoveX = -1;
			}
			else {
				controlMoveX = 0;
			}

			if (controlMoveY > 0) {
				controlMoveY = 1;
			}
			else if (controlMoveY < 0) {
				controlMoveY = -1;
			}
			else {
				controlMoveY = 0;
			}

		}
		else {
			// Si el ratón no está dentro del pad, desactiva el movimiento
			pad->clicked = false;
			controlMoveX = 0;
			controlMoveY = 0;
		}

		// Si el ratón no está sobre el botón de disparo, dejar de disparar
		if (!buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
	}

	// Si se suelta el botón del ratón
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			controlMoveX = 0;
			controlMoveY = 0;
		}

		// Si el ratón estaba sobre el botón de disparo, desactivar disparo
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}

		// Si el ratón está sobre el botón de escudo
		if (buttonShield->containsPoint(motionX, motionY)) {
			player->shieldActive = false; // Activar escudo
		}

		// Si el ratón está sobre el botón de congelación
		if (buttonFreezing->containsPoint(motionX, motionY)) {
			freezingAttack = false; // Activar ataque de congelación
		}

		// Si el ratón está sobre el botón de disparo
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
	}
}

void GameLayer2::gamePadToControls(SDL_Event event) {
	// Zona Muerta (Deadzone) para el joystick (ajustable)
	const int DEADZONE = 8000; // Puedes ajustar este valor dependiendo de lo sensible que quieras que sea el movimiento

	// Ejes para movimiento
	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
	int stickY = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTY);

	// Aplicar la zona muerta (deadzone)
	if (abs(stickX) < DEADZONE) {
		stickX = 0; // No mover si el eje está dentro de la zona muerta
	}
	if (abs(stickY) < DEADZONE) {
		stickY = 0; // No mover si el eje está dentro de la zona muerta
	}

	// Normalizar los valores del joystick a un rango de -1 a 1
	float controlMoveX = (float)stickX / 32767.0f;  // Normaliza el valor de -1 a 1
	float controlMoveY = (float)stickY / 32767.0f;  // Normaliza el valor de -1 a 1

	// Ajustar la velocidad de movimiento
	float movementSpeed = 0.5f;  // Puedes ajustar este valor para controlar la velocidad
	controlMoveX *= movementSpeed;
	controlMoveY *= movementSpeed;

	// Asignar el movimiento al jugador
	player->moveX(controlMoveX);
	player->moveY(controlMoveY);

	// Si el joystick se mueve hacia la derecha o hacia la izquierda
	if (stickX > DEADZONE) {
		controlMoveX = 1;  // Mover hacia la derecha
	}
	else if (stickX < -DEADZONE) {
		controlMoveX = -1;  // Mover hacia la izquierda
	}
	else {
		controlMoveX = 0;  // Detener movimiento en eje X si está dentro de la zona muerta
	}

	// Si el joystick se mueve hacia arriba o hacia abajo
	if (stickY > DEADZONE) {
		controlMoveY = 1;  // Mover hacia abajo
	}
	else if (stickY < -DEADZONE) {
		controlMoveY = -1;  // Mover hacia arriba
	}
	else {
		controlMoveY = 0;  // Detener movimiento en eje Y si está dentro de la zona muerta
	}

	// Botón para disparar
	if (SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A)) {
		controlShoot = true;  // Solo disparar si el botón está presionado
	}
	else {
		controlShoot = false;  // Desactivar disparo si el botón no está presionado
	}

	// Botón para activar escudo
	if (SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B)) {
		player->shieldActive = true;  // Activar escudo
	}
	else {
		player->shieldActive = false;  // Desactivar escudo
	}

	// Botón para ataque de hielo
	if (SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_X)) {
		freezingAttack = true;  // Activar ataque de hielo
	}
	else {
		freezingAttack = false;  // Desactivar ataque de hielo
	}

	// Botón para continuar o pausar
	if (SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_START)) {
		controlContinue = true;  // Activar control de continuar
	}
	else {
		controlContinue = false;  // Desactivar control de continuar si no se presiona
	}
}

void GameLayer2::update() {
	if (pause) return;

	if (checkLevelCompletion()) return;

	space->update();
	background->update();
	player->update();

	for (auto const& enemyProjectile : enemyProjectiles) {
		enemyProjectile->update();
	}
	for (auto const& playerProjectile : projectiles) {
		playerProjectile->update();
	}
	for (auto const& curacion : curaciones) {
		curacion->update();
	}
	for (auto const& municion : municiones) {
		municion->update();
	}

	updateGameObjects();
}

bool GameLayer2::checkLevelCompletion() {
	if (door->isPuzzleCompleted(boxes) && door->isOverlap(player)) {
		game->currentLevel++;
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5, WIDTH, HEIGHT, game);
		pause = true; game->layer = game->gameLayer3; return true;
	}
	return false;
}

void GameLayer2::updateGameObjects() {
	for (auto const& enemy : enemies) {
		enemy->update();
		Enemy1* enemy1 = dynamic_cast<Enemy1*>(enemy);

		if (enemy1 != nullptr) {
			Projectile* newProjectile = enemy1->autoShoot();
			if (newProjectile != nullptr) {
				space->addDynamicActor(newProjectile);
				enemyProjectiles.push_back(newProjectile);
			}
		}
	}

	handleEnemySeeingPlayer();
	handleEnemyTileCollisions();

	handlePlayerTileCollisions();
	handleProjectileCollisions();

	handlePlayerCollisions();
	handleEnemyProjectiles();

	handleCollectables();
	handleBoxCollisions();
}

void GameLayer2::handleEnemyProjectiles() {
	for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); ) {
		Projectile* projectile = *it;
		projectile->update();

		bool shouldRemove = false;
		for (auto const& tile : tiles) {
			if (projectile->isOverlap(tile)) {
				shouldRemove = true;
				break;
			}
		}

		if (shouldRemove) {
			space->removeDynamicActor(projectile);
			delete projectile;
			it = enemyProjectiles.erase(it);
		}
		else {
			++it;
		}
	}
}

void GameLayer2::handleEnemySeeingPlayer() {
	for (auto const& enemy : enemies) {
		if (auto enemy2 = dynamic_cast<Enemy2*>(enemy)) {
			if (enemy2->player == nullptr) {
				enemy2->player = player;
			}

			if (enemy2->canSeePlayer()) {
				enemy2->state = game->stateMoving;
			}
			else {
				enemy2->state = game->stateIdle;
			}
		}
	}
}

void GameLayer2::handleBoxCollisions() {
	for (auto& box : boxes) {
		if (player->isOverlap(box)) {

			box->isCollidingWithPlayer = true;
			bool canMoveX = true;

			float originalPlayerX = player->x;
			float originalPlayerY = player->y;
			float originalBoxX = box->x;
			float originalBoxY = box->y;
			float proposedBoxX = box->x + player->vx;
			float proposedBoxY = box->y;

			// Verificar colisiones en el eje X
			for (auto const& tile : tiles) {
				if (box->isOverlapWithProposed(tile, proposedBoxX, box->y)) {
					canMoveX = false;
					break;
				}
			}

			// Restringir movimiento adicional si interactúa con la puerta
			if (door->getState() == Door::DoorState::DOOR_SHUT) {
				if (box->isOverlapWithProposed(door, proposedBoxX, box->y)) {
					canMoveX = false;
				}
			}

			// Movimiento Horizontal (X)
			if (canMoveX && player->vx != 0) {
				box->x = proposedBoxX;
				if (player->vx > 0) {
					player->x = originalBoxX - player->width / 2 - box->width / 2;
				}
				else {
					player->x = originalBoxX + player->width / 2 + box->width / 2;
				}
			}
			else {
				player->x = originalPlayerX;
			}

			player->vx = originalPlayerX - player->x;
			player->vy = originalPlayerY - player->y;
		}
		else {
			box->isCollidingWithPlayer = false;
		}
	}
}

void GameLayer2::handlePlayerTileCollisions() {
	if (player->isOverlap(door)) {
		if (door->getState() == Door::DoorState::DOOR_SHUT) {
			float overlapLeft = (player->x + player->width / 2) - (door->x - door->width / 2);
			float overlapRight = (door->x + door->width / 2) - (player->x - player->width / 2);
			float overlapTop = (door->y + door->height / 2) - (player->y - player->height / 2);
			float overlapBottom = (player->y + player->height / 2) - (door->y - door->height / 2);

			float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

			if (minOverlap == overlapBottom) {
				player->y = door->y - door->height / 2 - player->height / 2;
				if (controlMoveY > 0) controlMoveY = 0;
			}
			else if (minOverlap == overlapTop) {
				player->y = door->y + door->height / 2 + player->height / 2;
				if (controlMoveY < 0) controlMoveY = 0;
			}
			else if (minOverlap == overlapLeft) {
				player->x = door->x - door->width / 2 - player->width / 2;
				if (controlMoveX > 0) controlMoveX = 0;
			}
			else if (minOverlap == overlapRight) {
				player->x = door->x + door->width / 2 + player->width / 2;
				if (controlMoveX < 0) controlMoveX = 0;
			}
		}
	}

	for (auto const& tile : tiles) {
		if (player->isOverlap(tile)) {
			resolvePlayerTileCollision(tile);
		}
	}
}

void GameLayer2::resolvePlayerTileCollision(Tile* tile) {
	float overlapLeft = (player->x + player->width / 2) - (tile->x - tile->width / 2);
	float overlapRight = (tile->x + tile->width / 2) - (player->x - player->width / 2);
	float overlapTop = (tile->y + tile->height / 2) - (player->y - player->height / 2);
	float overlapBottom = (player->y + player->height / 2) - (tile->y - tile->height / 2);

	float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	if (minOverlap == overlapBottom) {
		player->y = tile->y - tile->height / 2 - player->height / 2;
		if (controlMoveY > 0) controlMoveY = 0;
	}
	else if (minOverlap == overlapTop) {
		player->y = tile->y + tile->height / 2 + player->height / 2;
		if (controlMoveY < 0) controlMoveY = 0;
	}
	else if (minOverlap == overlapLeft) {
		player->x = tile->x - tile->width / 2 - player->width / 2;
		if (controlMoveX > 0) controlMoveX = 0;
	}
	else if (minOverlap == overlapRight) {
		player->x = tile->x + tile->width / 2 + player->width / 2;
		if (controlMoveX < 0) controlMoveX = 0;
	}
}

void GameLayer2::handleEnemyTileCollisions() {
	for (auto enemy : enemies) {
		for (auto tile : tiles) {
			if (enemy->isOverlap(tile)) {
				if (auto enemy1 = dynamic_cast<Enemy1*>(enemy)) {
					resolveEnemy1TileCollision(enemy1, tile);
				}
				else if (auto enemy2 = dynamic_cast<Enemy2*>(enemy)) {
					resolveEnemy2TileCollision(enemy2, tile);
				}
			}
		}
	}
}

void GameLayer2::resolveEnemy1TileCollision(Enemy1* enemy, Tile* tile) {
	// Ajustar la escala de colisión
	float collisionScale = 0.20f;
	float adjustedWidth = enemy->width * collisionScale;
	float adjustedHeight = enemy->height * collisionScale;

	// Bordes ajustados del enemigo y del tile
	float enemyLeft = enemy->x - adjustedWidth / 2;
	float enemyRight = enemy->x + adjustedWidth / 2;
	float tileLeft = tile->x - tile->width / 2;
	float tileRight = tile->x + tile->width / 2;

	// Calcular solapamientos en el eje X
	float overlapLeft = enemyRight - tileLeft;
	float overlapRight = tileRight - enemyLeft;

	// Si hay colisión en el eje X
	if (overlapLeft > 0 || overlapRight > 0) {
		enemy->orientation = (overlapLeft > 0) ? 1 : -1;
	}
}

void GameLayer2::resolveEnemy2TileCollision(Enemy2* enemy, Tile* tile) {
	float collisionScale = 0.20f;
	float adjustedWidth = enemy->width * collisionScale;
	float adjustedHeight = enemy->height * collisionScale;

	// Bordes ajustados del enemigo
	float enemyLeft = enemy->x - adjustedWidth / 2;
	float enemyRight = enemy->x + adjustedWidth / 2;
	float enemyTop = enemy->y - adjustedHeight / 2;
	float enemyBottom = enemy->y + adjustedHeight / 2;

	// Bordes del tile
	float tileLeft = tile->x - tile->width / 2;
	float tileRight = tile->x + tile->width / 2;
	float tileTop = tile->y - tile->height / 2;
	float tileBottom = tile->y + tile->height / 2;

	// Calcular solapamientos
	float overlapLeft = enemyRight - tileLeft;
	float overlapRight = tileRight - enemyLeft;
	float overlapTop = enemyBottom - tileTop;
	float overlapBottom = tileBottom - enemyTop;

	// Detectar colisiones por cada lado
	bool collisionLeft = overlapLeft > 0 && overlapLeft < tile->width && enemyBottom > tileTop && enemyTop < tileBottom;
	bool collisionRight = overlapRight > 0 && overlapRight < tile->width && enemyBottom > tileTop && enemyTop < tileBottom;
	bool collisionTop = overlapTop > 0 && overlapTop < tile->height && enemyRight > tileLeft && enemyLeft < tileRight;
	bool collisionBottom = overlapBottom > 0 && overlapBottom < tile->height && enemyRight > tileLeft && enemyLeft < tileRight;

	// Manejar colisiones (ajuste si necesario)
	if (collisionBottom) {
		// Si el enemigo está sobre el tile, ajustamos su posición en el eje Y.
		enemy->y += overlapBottom;  // Empujar al enemigo hacia arriba para evitar que atraviese el suelo
		enemy->vy = enemy->vyIntelligence;  // Mantener o ajustar la velocidad vertical
	}
	else if (collisionTop) {
		// Si el enemigo choca con el techo, empujamos hacia abajo.
		enemy->y -= overlapTop;
		enemy->vy = -enemy->vyIntelligence;  // Invertir la velocidad vertical
	}
	else if (collisionLeft) {
		// Si el enemigo colisiona con el lado izquierdo del tile
		enemy->x += overlapLeft;  // Empujar al enemigo a la derecha
		enemy->vx = -enemy->vxIntelligence;  // Invertir la dirección horizontal
	}
	else if (collisionRight) {
		// Si el enemigo colisiona con el lado derecho del tile
		enemy->x -= overlapRight;
		enemy->vx = enemy->vxIntelligence;
	}

	if ((collisionLeft || collisionRight) && (collisionTop || collisionBottom)) {
		if (abs(overlapLeft) > abs(overlapTop) || abs(overlapRight) > abs(overlapTop)) {
			enemy->x += (collisionLeft ? overlapLeft : -overlapRight);
		}
		else {
			enemy->y += (collisionTop ? overlapTop : -overlapBottom);
		}
	}
}

void GameLayer2::handlePlayerCollisions() {
	list<BaseEnemy*> deleteEnemies;
	bool shouldRemoveProjectile = false;

	for (auto const& enemyP : enemyProjectiles) {
		if (player->isOverlap(enemyP)) {
			shouldRemoveProjectile = true;
			if (!player->shieldActive && player->invulnerableTime <= 0.0f) {
				player->lifes--;
				vidasJugador = player->lifes;
				textVidasJugador->content = to_string(vidasJugador);

				player->invulnerableTime = 5.0f;
				if (player->lifes <= 0) { init(); return; }
			}
		}

		if (shouldRemoveProjectile) {
			space->removeDynamicActor(enemyP);
			delete enemyP;
			enemyProjectiles.remove(enemyP);
			break;
		}
	}

	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy) && !enemy->isFrozen) {
			init(); return;
		}
	}
}

void GameLayer2::handleCollectables() {
	list<Recolectable*> deleteMuniciones;
	list<Recolectable*> deleteCuraciones;

	for (auto const& municion : municiones) {
		if (player->isOverlap(municion)) {
			if (std::find(deleteMuniciones.begin(), deleteMuniciones.end(), municion)
				== deleteMuniciones.end()) {
				deleteMuniciones.push_back(municion);
			}
			municionCogida++;
			player->numberOfShots++;
			textMunicionCogida->content = to_string(municionCogida);
		}
	}

	for (auto const& delmunicion : deleteMuniciones) {
		municiones.remove(delmunicion);
		space->removeDynamicActor(delmunicion);
	}

	for (auto const& curacion : curaciones) {
		if (player->isOverlap(curacion)) {
			if (std::find(deleteCuraciones.begin(), deleteCuraciones.end(), curacion)
				== deleteCuraciones.end()) {
				deleteCuraciones.push_back(curacion);
			}

			vidasJugador++;
			player->lifes++;
			textVidasJugador->content = to_string(vidasJugador);
		}
	}

	for (auto const& delcuracion : deleteCuraciones) {
		curaciones.remove(delcuracion);
		space->removeDynamicActor(delcuracion);
	}
}

void GameLayer2::handleProjectileCollisions() {
	list<Projectile*> deletePlayerProjectiles;
	list<Projectile*> deleteFreezingProjectiles;
	list<BaseEnemy*> deleteEnemies;

	for (auto const& projectile : projectilesFreeze) {
		for (auto const& enemy : enemies) {
			if (projectile->isOverlap(enemy)) {
				enemy->isFrozen = true; enemy->timeFrozen = 120.0f;
				enemy->vx = 0; enemy->vy = 0;
				deleteFreezingProjectiles.push_back(projectile);
				break;
			}
		}

		for (auto const& tile : tiles) {
			if (projectile->isOverlap(tile)) {
				deleteFreezingProjectiles.push_back(projectile);
				break;
			}
		}
	}

	for (auto const& projectile : projectiles) {
		for (auto const& enemy : enemies) {
			if (projectile->isOverlap(enemy)) {
				enemy->state = game->stateDying; enemigosEliminados++;
				textEnemigosEliminados->content = to_string(enemigosEliminados);

				if (enemigosEliminados % 10 == 0) {
					ataquesHielo++; player->numberOfFreezingAttacks++;
					textAtaquesHielo->content = to_string(ataquesHielo);
				}

				deletePlayerProjectiles.push_back(projectile);
				deleteEnemies.push_back(enemy);
				break;
			}
		}

		for (auto const& tile : tiles) {
			if (projectile->isOverlap(tile)) {
				deletePlayerProjectiles.push_back(projectile);
				break;
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
		delete delEnemy;
	}

	for (auto const& delFreezingProjectile : deleteFreezingProjectiles) {
		projectilesFreeze.remove(delFreezingProjectile);
		space->removeDynamicActor(delFreezingProjectile);
		delete delFreezingProjectile;
	}

	for (auto const& delPlayerProjectile : deletePlayerProjectiles) {
		projectiles.remove(delPlayerProjectile);
		space->removeDynamicActor(delPlayerProjectile);
		delete delPlayerProjectile;
	}
}

void GameLayer2::calculateScroll() {
	if (player->x > WIDTH * 0.5) {
		if (player->x - scrollX < WIDTH * 0.5) {
			scrollX = player->x - WIDTH * 0.5;
		}
	}

	if (player->x < mapWidth - WIDTH * 0.5) {
		if (player->x - scrollX > WIDTH * 0.5) {
			scrollX = player->x - WIDTH * 0.5;
		}
	}

	if (player->y > HEIGHT * 0.5) {
		if (player->y - scrollY < HEIGHT * 0.5) {
			scrollY = player->y - HEIGHT * 0.5;
		}
	}

	if (player->y - scrollY < HEIGHT * 0.4) {
		scrollY = player->y - HEIGHT * 0.4;
	}

	if (player->y - scrollY > HEIGHT * 0.6) {
		scrollY = player->y - HEIGHT * 0.6;
	}
}

void GameLayer2::draw() {
	calculateScroll();
	background->draw();

	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}
	for (auto const& projectile : projectilesFreeze) {
		projectile->draw(scrollX, scrollY);
	}
	for (auto const& enemyProjectile : enemyProjectiles) {
		enemyProjectile->draw(scrollX, scrollY);
	}
	for (auto const& municion : municiones) {
		municion->draw(scrollX, scrollY);
	}
	for (auto const& curacion : curaciones) {
		curacion->draw(scrollX, scrollY);
	}
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}
	for (auto const& box : boxes) {
		box->draw(scrollX, scrollY);
	}

	door->draw(scrollX, scrollY);
	player->draw(scrollX, scrollY);

	textVidasJugador->draw();
	textAtaquesHielo->draw();
	textMunicionCogida->draw();
	textEnemigosEliminados->draw();

	backgroundVidasJugador->draw();
	backgroundAtaquesHielo->draw();
	backgroundEnemigosEliminados->draw();
	backgroundMunicionCogida->draw();

	if (player->shieldActive) { backgroundEscudo->draw(); }
	if (pause) { message->draw(); }
	SDL_RenderPresent(game->renderer);
}


void GameLayer2::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo 
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // disparar
			controlShoot = true;
			break;
		case SDLK_f: // escudo
			player->shieldActive = true;
			break;
		case SDLK_y: // ataque de hielo
			freezingAttack = true;
			break;
		}
	}

	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		case SDLK_f: // escudo
			player->shieldActive = false;
			break;
		case SDLK_y: // ataque de hielo
			freezingAttack = false;
			break;
		}
	}
}