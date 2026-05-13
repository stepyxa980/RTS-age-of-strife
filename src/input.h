#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"

typedef struct {
    bool keys[MAX_KEYS];
    bool key_just_pressed[MAX_KEYS];
    bool key_just_released[MAX_KEYS];

    float mouse_x, mouse_y;
    bool mouse_buttons[MAX_MOUSE_BTNS];
    bool mouse_just_pressed[MAX_MOUSE_BTNS];
    bool mouse_just_released[MAX_MOUSE_BTNS];
    int mouse_wheel;

    bool quit_requested;
} InputState;

void input_init(InputState* input);
void handle_events(InputState *input, bool *running);
void input_clear(InputState *input);