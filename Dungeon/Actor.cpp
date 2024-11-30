#include "Actor.h"

Actor::~Actor() {
	
}

Actor::Actor(string filename, float x, float y, int width, int height, Game* game) {
	clicked = false;
	this->game = game;
	texture = game->getTexture(filename);
	this->x = x;
	this->y = y;
	// lo que mide el fichero
	this->fileWidth = width;
	this->fileHeight = height;
	// lo que va a medir en el juego
	this->width = width;
	this->height = height;
}

bool Actor::isOverlapWithProposed(Actor* other, float proposedX, float proposedY) {
	float leftA = proposedX - this->width / 2;
	float rightA = proposedX + this->width / 2;
	float topA = proposedY - this->height / 2;
	float bottomA = proposedY + this->height / 2;

	float leftB = other->x - other->width / 2;
	float rightB = other->x + other->width / 2;
	float topB = other->y - other->height / 2;
	float bottomB = other->y + other->height / 2;

	return !(leftA >= rightB || rightA <= leftB || topA >= bottomB || bottomA <= topB);
}

void Actor::draw(float scrollX, float scrollY) {
	// Recorte en el fichero de la imagen
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = fileWidth;  
	source.h = fileHeight; 

	// Donde se va a pegar en el renderizador
	SDL_Rect destination;
	destination.x = x - width / 2 - scrollX;  
	destination.y = y - height / 2 - scrollY; 
	destination.w = width;                      
	destination.h = height;                     

	// Renderizar la textura en la posición calculada
	SDL_RenderCopyEx(game->renderer,
		texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
}


bool Actor::isOverlap(Actor* actor) {
	bool overlap = false;
	if (actor->x - actor->width / 2 <= x + width / 2
		&& actor->x + actor->width / 2 >= x - width / 2
		&& actor->y + actor->height / 2 >= y - height / 2
		&& actor->y - actor->height / 2 <= y + height / 2) {

		overlap = true;
	}

	return overlap;
}

bool Actor::isFrontOverlap(Actor* actor, int orientation) {
	float detectionOffset = 2.0f; // Ajuste pequeño para el área de detección
	float detectionX = (orientation > 0) ? actor->x + actor->width / 2 + detectionOffset // Derecha
		: actor->x - actor->width / 2 - detectionOffset; // Izquierda

	// Comprobar si el actor está dentro del rango frontal
	bool overlap = false;
	if (actor->x - actor->width / 2 <= detectionX &&
		actor->x + actor->width / 2 >= detectionX &&
		actor->y + actor->height / 2 >= actor->y - actor->height / 2 &&
		actor->y - actor->height / 2 <= actor->y + actor->height / 2) {
		overlap = true;
	}

	return overlap;
}


bool Actor::isInRender(float scrollX) {
	if ((x - scrollX) - width / 2 <= WIDTH && (x - scrollX) + width / 2 >= 0 &&
		y - height / 2 <= HEIGHT && y + height / 2 >= 0) {
		return true;
	}
	return false;
}


bool Actor::containsPoint(int pointX, int pointY) {
	if (pointY >= y - height / 2 &&
		pointY <= y + height / 2 &&
		pointX <= x + width / 2 &&
		pointX >= x - width / 2) {
		return true;
	}
	return false;
}



