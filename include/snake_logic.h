#define SNAKE_LOGIC_H

#include <SDL2/SDL.h>


typedef struct SnakeSegment {
    int x;
    int y;
    char direction;
    char prev_direction;
    struct SnakeSegment *next;
    struct SnakeSegment *prev;
} SnakeSegment;

typedef struct {
    SnakeSegment *head;
    SnakeSegment *tail;
    char direction;
    char prev_direction;
} Snake;

void move_snake(Snake *snake);
Snake *init_snake (int start_x, int start_y, int start_length, char start_direction);
void move_snake(Snake *snake);
void grow_snake(Snake *snake);
void shrink_snake(Snake *snake);
bool self_collision(SnakeSegment *head, int head_x, int head_y);
