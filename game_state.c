#include "game_state.h"

GameState game = {
    .window_width = 800,
    .window_height = 600,
    .cell_size = 50,
    .running = 1,
    .window = NULL,
    .renderer = NULL,
    .snake_speed = 100
};