#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game_state.h"
#include "snake_logic.h"

int main () {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake Game",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         game.window_width, game.window_height, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    Uint32 last_timestamp = 0;
    Snake *snake = init_snake(50, 50, 3, 'R');
    while (game.running) {
        frameStart = SDL_GetTicks();
        

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (snake->direction != 'D') snake->direction = 'U';
                        break;
                    case SDLK_s:
                        if (snake->direction != 'U') snake->direction = 'D';
                        break;
                    case SDLK_d:
                        if (snake->direction != 'L') snake->direction = 'R';
                        break;
                    case SDLK_a:
                        if (snake->direction != 'R') snake->direction = 'L';
                        break;
                }
            }
        }
        if (SDL_GetTicks() - last_timestamp >= game.snake_speed) {
            move_snake(snake);
            last_timestamp = SDL_GetTicks();
        }
        

        // Draw black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SnakeSegment *currentHead = snake->head;
       while (currentHead != NULL) {
            SDL_Rect seg = {currentHead->x * 10, currentHead->y * 10, 10, 10};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &seg);
            currentHead = currentHead->next;
       }
            
     
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks();
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}