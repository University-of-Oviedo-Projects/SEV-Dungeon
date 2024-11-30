#pragma once
#include "Layer.h"
#include "Actor.h"
#include "Background.h"
#include "Text.h"

class WinLayer : public Layer
{
public:
	WinLayer(Game* game);
	void init() override;
	void draw() override;
	void processControls() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);
	void gamePadToControls(SDL_Event event);
	bool controlContinue;
	SDL_GameController* gamePad; 
	Background* background;
	Text* message;
};


