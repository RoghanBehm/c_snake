#include <stdio.h>
#include <stdbool.h>
#include "snake_logic.h"
#include "game_state.h"

Snake *init_snake (int start_x, int start_y, int start_length, char start_direction) {
    Snake *snake = malloc(sizeof(Snake));
    snake->head = NULL;
    snake->tail = NULL;
    snake->direction = start_direction;
    snake->prev_direction;

    for (int i = 0; i < start_length; i++) {
        SnakeSegment *new_segment = malloc(sizeof(SnakeSegment));
        new_segment->x = start_x - (i * game.cell_size);
        new_segment->y = start_y;
        new_segment->direction = start_direction;
        new_segment->prev_direction = start_direction;
        new_segment->next = NULL;
        new_segment->prev = NULL;

        if (snake->head == NULL) {
            snake->head = new_segment;
            snake->tail = new_segment;
        } else {
            new_segment->next = snake->head;
            snake->head->prev = new_segment;
            snake->head = new_segment;
        }

    }
    return snake;

}

void move_snake(Snake *snake) {

    int new_x = snake->head->x;
    int new_y = snake->head->y;

    switch (snake->direction) {
        case 'U': new_y -= 1; break;
        case 'D': new_y += 1; break;
        case 'L': new_x -= 1; break;
        case 'R': new_x += 1; break;
    }

    SnakeSegment *new_head = malloc(sizeof(SnakeSegment));
    new_head->x = new_x;
    new_head->y = new_y;
    new_head->direction = snake->direction;
    new_head->prev_direction = snake->prev_direction;
    new_head->prev = NULL;
    new_head->next = snake->head;

    if(snake->head) {
        snake->head->prev = new_head;
        snake->head->direction = snake->direction;
    }
    snake->head = new_head;

    SnakeSegment *old_tail = snake->tail;
    snake->tail = old_tail->prev;
    snake->tail->next = NULL;
    free(old_tail);
}

void grow_snake(Snake *snake) {
    int new_x = snake->head->x;
    int new_y = snake->head->y;

    switch (snake->direction) {
        case 'U': new_y -= 1; break;
        case 'D': new_y += 1; break;
        case 'L': new_x -= 1; break;
        case 'R': new_x += 1; break;
    }

    SnakeSegment *new_head = malloc(sizeof(SnakeSegment));
    new_head->x = new_x;
    new_head->y = new_y;
    new_head->prev = NULL;
    new_head->next = snake->head;
    snake->head->prev = new_head;
    snake->head = new_head;
}

void shrink_snake(Snake *snake) {

    if (snake->head == snake->tail) { // Does not shrink if snake is only 1 segment long
        return; 
    }
    SnakeSegment *old_tail = snake->tail;
    snake->tail = old_tail->prev;
    snake->tail->next = NULL;
    free(old_tail);
}

bool self_collision(SnakeSegment *head, int head_x, int head_y) {
    SnakeSegment *current = head->next;
    while (current != NULL) {
        if (head_x == current->x && head_y == current->y) {
            return true;
        }
        current = current->next;
    }
    return false;
}
