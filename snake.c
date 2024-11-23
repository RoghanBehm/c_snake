#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>   
#include <stdbool.h>
#include "game_state.h"
#include "snake_logic.h"

bool apple_flag = false;
bool pApple_flag = false;
bool direction_changed = false;
int score = 0;

int random_in_range(int min, int max) {
    return min + rand() % (max - min + 1);
}


void place_apple(SDL_Renderer *renderer, Sint32 *apple_x, Sint32 *apple_y) {
    *apple_x = random_in_range(0, 79);
    *apple_y = random_in_range(0, 59);
}

void place_pApple(SDL_Renderer *renderer, Sint32 *pApple_x, Sint32 *pApple_y) {
    *pApple_x = random_in_range(0, 79);
    *pApple_y = random_in_range(0, 59);
}

int main () {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("TTF init failed: %s\n", TTF_GetError());
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

    TTF_Font *font = TTF_OpenFont("fonts/Roboto-Thin.ttf", 24);
    if (!font) {
        printf("Font didn't load :-()");
        return 1;
    }

    SDL_Color text_color = {255, 255, 255, 255};
    


    SDL_Event event;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    Uint32 last_timestamp = 0;
    Snake *snake = init_snake(50, 50, 3, 'R');
    Sint32 apple_x = 0;
    Sint32 apple_y = 0;
    Sint32 pApple_x = 0;
    Sint32 pApple_y = 0;
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
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
        SDL_RenderFillRect(renderer, &apple);

        // Draw poison apple
        SDL_Rect pApple = {pApple_x * 10, pApple_y * 10, 10, 10};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pApple);

        if (!apple_flag) {
            place_apple(renderer, &apple_x, &apple_y);
            apple_flag = true;
        }
        
        if (!pApple_flag) {
            place_pApple(renderer, &pApple_x, &pApple_y);
            pApple_flag = true;
        }

        // Draw segments
        SnakeSegment *currentHead = snake->head;
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
            score++;
        }

        // Poison apple collision check
        if (snake->head->x == pApple_x && snake->head->y == pApple_y) {
            shrink_snake(snake);
            place_pApple(renderer, &pApple_x, &pApple_y);
            game.snake_speed += 5;
            score--;

        }

        char score_text[50];
        sprintf(score_text, "Score: %d", score);
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, score_text, text_color);
        if (!text_surface) {
            printf("Failed to create txt surface");
            return 1;
        }

        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);

        if (!text_texture) {
            printf("Text texture failed");
            return 1;
        }
        // Draw score
        SDL_Rect text_rect = {10, 10, 150, 50};
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

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

    
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}