#define GAME_STATE_H

#include <SDL2/SDL.h>

typedef struct {
    int window_width;
    int window_height;
    int cell_size;
    int running;
    SDL_Window *window;
    SDL_Renderer *renderer;
} GameState;

extern GameState game;