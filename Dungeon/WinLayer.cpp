#include "WinLayer.h"
#include "Text.h"

WinLayer::WinLayer(Game* game)
    : Layer(game) {
    init();
}

void WinLayer::init() {
    // Fondo del menú de victoria
    background = new Background("res/menu_fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);
    message = new Text("You Win! Congratulations!", WIDTH * 0.5, HEIGHT * 0.7, game);
    gamePad = SDL_GameControllerOpen(0);
}

void WinLayer::draw() {
    background->draw();
    message->draw();

    SDL_RenderPresent(game->renderer); 
}

void WinLayer::keysToControls(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        int code = event.key.keysym.sym;
        // Permitir solo salir con la tecla ESC
        if (code == SDLK_ESCAPE) {
            game->loopActive = false;
        }
    }
}

void WinLayer::mouseToControls(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        // Salir al hacer clic en cualquier lugar
        game->loopActive = false;
    }
}


void WinLayer::gamePadToControls(SDL_Event event) {
    // Leer los botones del GamePad
    if (SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_START)) {
        game->loopActive = false; // Salir al pulsar START
    }
}

void WinLayer::processControls() {
    // Obtener controles
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERDEVICEADDED) {
            gamePad = SDL_GameControllerOpen(0);
            if (gamePad == NULL) {
                cout << "Error al conectar el GamePad" << endl;
            }
            else {
                cout << "GamePad conectado" << endl;
            }
        }
        // Detectar input activo
        if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
            game->input = game->inputGamePad;
        }
        if (event.type == SDL_KEYDOWN) {
            game->input = game->inputKeyboard;
        }
        // Procesar controles según el tipo de entrada
        if (game->input == game->inputGamePad) {
            gamePadToControls(event);
        }
        if (game->input == game->inputKeyboard) {
            keysToControls(event);
        }
    }
}
