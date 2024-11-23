#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game_state.h"
#include "snake_logic.h"

bool apple_flag = false;
bool direction_changed = false;

int random_in_range(int min, int max) {
    return min + rand() % (max - min + 1);
}


void place_apple(SDL_Renderer *renderer, Sint32 *apple_x, Sint32 *apple_y) {
    *apple_x = random_in_range(0, 79);
    *apple_y = random_in_range(0, 59);
}

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
    Sint32 apple_x = 0;
    Sint32 apple_y = 0;
    while (game.running) {
        frameStart = SDL_GetTicks();
        

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game.running = false;
            }

            if (event.type == SDL_KEYDOWN && !direction_changed) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (snake->direction != 'D') {
                            snake->direction = 'U';
                            direction_changed = true;
                            break;
                        }
                            
                    case SDLK_s:
                        if (snake->direction != 'U') {
                            snake->direction = 'D';
                            direction_changed = true;
                            break;
                        }
                    case SDLK_d:
                        if (snake->direction != 'L') {
                            snake->direction = 'R';
                            direction_changed = true;
                            break;
                        }
                            
                    case SDLK_a:
                        if (snake->direction != 'R') {
                           snake->direction = 'L';
                            direction_changed = true;
                            break; 
                        } 
                }
            }
        }
        if (SDL_GetTicks() - last_timestamp >= game.snake_speed) {
            move_snake(snake);
            direction_changed = false;
            last_timestamp = SDL_GetTicks();
        }
        

        
        
        // Draw black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw apple
        SDL_Rect apple = {apple_x * 10, apple_y * 10, 10, 10};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &apple);

        if (!apple_flag) {
            place_apple(renderer, &apple_x, &apple_y);
            apple_flag = true;
        }
        
        
        SnakeSegment *currentHead = snake->head;
        // Draw segments
        while (currentHead != NULL) {
            SDL_Rect seg = {currentHead->x * 10, currentHead->y * 10, 10, 10};
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &seg);
            currentHead = currentHead->next;
        }
        
        // Apple collision check
        if (snake->head->x == apple_x && snake->head->y == apple_y) {
            grow_snake(snake);
            place_apple(renderer, &apple_x, &apple_y);
            game.snake_speed -= 5;
        }

        //  Snake segment collision check
        if (self_collision(snake->head, snake->head->x, snake->head->y)) {
            game.running = false;
            printf("you died");
        }   

        // Wall collision check
        if (snake->head->x < 0 || snake->head->x >= 80 || snake->head->y < 0 || snake->head->y >= 60) {
            game.running = false;
            printf("you died");
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