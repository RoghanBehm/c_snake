#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "game_state.h"
#include "snake_logic.h"

#define GRID_WIDTH (game.window_width / game.cell_size)
#define GRID_HEIGHT (game.window_height / game.cell_size)

bool apple_flag = false;
bool pApple_flag = false;
bool direction_changed = false;
int score = 0;

int random_in_range(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void place_apple(SDL_Renderer *renderer, Sint32 *apple_x, Sint32 *apple_y)
{
    *apple_x = random_in_range(0, GRID_WIDTH - 1);
    *apple_y = random_in_range(0, GRID_HEIGHT - 1);
}

void place_pApple(SDL_Renderer *renderer, Sint32 *pApple_x, Sint32 *pApple_y)
{
    *pApple_x = random_in_range(0, GRID_WIDTH - 1);
    *pApple_y = random_in_range(0, GRID_HEIGHT - 1);
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0)
    {
        printf("TTF init failed: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake Game",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          game.window_width, game.window_height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("fonts/Roboto-Thin.ttf", 24);
    if (!font)
    {
        printf("Font didn't load :-()");
        return 1;
    }

    SDL_Texture *cherry_img = IMG_LoadTexture(renderer, "assets/cerry.png");
    if (!cherry_img)
    {
        printf("Failed to load apple texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *frog_img = IMG_LoadTexture(renderer, "assets/frog.png");
    if (!frog_img)
    {
        printf("Failed to load frog_img texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *frog_right = IMG_LoadTexture(renderer, "assets/frog90.png");
    if (!frog_right)
    {
        printf("Failed to load frog_right texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *frog_down = IMG_LoadTexture(renderer, "assets/frog180.png");
    if (!frog_down)
    {
        printf("Failed to load frog_down texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *frog_left = IMG_LoadTexture(renderer, "assets/frog270.png");
    if (!frog_left)
    {
        printf("Failed to load frog_left texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *turn_LDorUR = IMG_LoadTexture(renderer, "assets/turn_LDorUR.png");
    if (!turn_LDorUR)
    {
        printf("Failed to load turn_LDorUR texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *turn_LUorDR = IMG_LoadTexture(renderer, "assets/turn_LUorDR.png");
    if (!turn_LUorDR)
    {
        printf("Failed to load turn_LUorDR texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *turn_RDorUL = IMG_LoadTexture(renderer, "assets/turn_RDorUL.png");
    if (!turn_RDorUL)
    {
        printf("Failed to load turn_RDorUL texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *turn_RUorDL = IMG_LoadTexture(renderer, "assets/turn_RUorDL.png");
    if (!turn_RUorDL)
    {
        printf("Failed to load turn_RUorDL texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *frog_body = IMG_LoadTexture(renderer, "assets/body.png");
    if (!frog_body)
    {
        printf("Failed to load frog_body texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Color text_color = {255, 255, 255, 255};

    SDL_Event event;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    Uint32 last_timestamp = 0;
    Snake *snake = init_snake(100, 0, 3, 'R');
    Sint32 apple_x = 0;
    Sint32 apple_y = 0;
    Sint32 pApple_x = 0;
    Sint32 pApple_y = 0;
    while (game.running)
    {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game.running = false;
            }

            if (event.type == SDL_KEYDOWN && !direction_changed)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    if (snake->direction != 'D')
                    {
                        snake->prev_direction = snake->direction;
                        snake->direction = 'U';
                        direction_changed = true;
                        break;
                    }

                case SDLK_s:
                    if (snake->direction != 'U')
                    {
                        snake->prev_direction = snake->direction;
                        snake->direction = 'D';
                        direction_changed = true;
                        break;
                    }
                case SDLK_d:
                    if (snake->direction != 'L')
                    {
                        snake->prev_direction = snake->direction;
                        snake->direction = 'R';
                        direction_changed = true;
                        break;
                    }

                case SDLK_a:
                    if (snake->direction != 'R')
                    {
                        snake->prev_direction = snake->direction;
                        snake->direction = 'L';
                        direction_changed = true;
                        break;
                    }
                }
            }
        }
        if (SDL_GetTicks() - last_timestamp >= game.snake_speed)
        {
            move_snake(snake);
            direction_changed = false;
            last_timestamp = SDL_GetTicks();
        }

        // Draw black background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw apple
        SDL_Rect apple = {apple_x * game.cell_size, apple_y * game.cell_size, game.cell_size, game.cell_size};
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
        SDL_RenderCopy(renderer, cherry_img, NULL, &apple);

        // Draw poison apple
        SDL_Rect pApple = {pApple_x * game.cell_size, pApple_y * game.cell_size, game.cell_size, game.cell_size};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &pApple);

        if (!apple_flag)
        {
            place_apple(renderer, &apple_x, &apple_y);
            apple_flag = true;
        }

        if (!pApple_flag)
        {
            place_pApple(renderer, &pApple_x, &pApple_y);
            pApple_flag = true;
        }

        typedef struct
        {
            char dir;
            char prev_dir;
            SDL_Texture *image;
        } TurnMap;

        TurnMap turnAssets[] = {
            {'U', 'U', frog_body},
            {'U', 'R', turn_LDorUR},
            {'U', 'L', turn_RDorUL},
            {'R', 'R', frog_body},
            {'R', 'U', turn_RUorDL},
            {'R', 'D', turn_RDorUL},
            {'D', 'D', frog_body},
            {'D', 'L', turn_RUorDL},
            {'D', 'R', turn_LUorDR},
            {'L', 'L', frog_body},
            {'L', 'D', turn_LDorUR},
            {'L', 'U', turn_LUorDR}};

        // Draw segments
        SnakeSegment *currentHead = snake->head;
        while (currentHead != NULL)
        {
            SDL_Rect seg = {currentHead->x * game.cell_size, currentHead->y * game.cell_size, game.cell_size, game.cell_size};
            if (currentHead->prev == NULL)
            {
                switch (snake->direction)
                {
                case 'U':
                    SDL_RenderCopy(renderer, frog_img, NULL, &seg);
                    break;
                case 'R':
                    SDL_RenderCopy(renderer, frog_right, NULL, &seg);
                    break;
                case 'D':
                    SDL_RenderCopy(renderer, frog_down, NULL, &seg);
                    break;
                case 'L':
                    SDL_RenderCopy(renderer, frog_left, NULL, &seg);
                }
            }else if (currentHead->next == NULL) {  // Tail segment
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &seg);
    } else {  // Body segment
        char current_dir = currentHead->direction;
        char next_dir = currentHead->next->direction;

        if (current_dir != next_dir) {  // Turn detected
            for (int i = 0; i < sizeof(turnAssets) / sizeof(turnAssets[0]); i++) {
                if (turnAssets[i].prev_dir == current_dir &&
                    turnAssets[i].dir == next_dir) {
                    SDL_RenderCopy(renderer, turnAssets[i].image, NULL, &seg);
                    break;
                }
            }
        } else {  // Straight body segment
            SDL_RenderCopy(renderer, frog_body, NULL, &seg);
        }
    }

    currentHead = currentHead->next;
}

        // Apple collision check
        if (snake->head->x == apple_x && snake->head->y == apple_y)
        {
            grow_snake(snake);
            place_apple(renderer, &apple_x, &apple_y);
            game.snake_speed -= 5;
            score++;
        }

        // Poison apple collision check
        if (snake->head->x == pApple_x && snake->head->y == pApple_y)
        {
            shrink_snake(snake);
            place_pApple(renderer, &pApple_x, &pApple_y);
            game.snake_speed += 5;
            score--;
        }

        char score_text[50];
        sprintf(score_text, "Score: %d", score);
        SDL_Surface *text_surface = TTF_RenderText_Solid(font, score_text, text_color);
        if (!text_surface)
        {
            printf("Failed to create txt surface");
            return 1;
        }

        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        SDL_FreeSurface(text_surface);

        if (!text_texture)
        {
            printf("Text texture failed");
            return 1;
        }
        // Draw score
        SDL_Rect text_rect = {10, 10, 150, 50};
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

        //  Snake segment collision check
        if (self_collision(snake->head, snake->head->x, snake->head->y))
        {
            game.running = false;
            printf("you died");
        }

        // Wall collision check
        if (snake->head->x < 0 || snake->head->x >= GRID_WIDTH ||
            snake->head->y < 0 || snake->head->y >= GRID_HEIGHT)
        {
            game.running = false;
            printf("you died");
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks();
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyTexture(cherry_img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}