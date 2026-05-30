#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"
#include "camera.h"

typedef struct {
    bool keys[MAX_KEYS];
    bool key_just_pressed[MAX_KEYS];
    bool key_just_released[MAX_KEYS];

    float mouse_x, mouse_y;
    float world_mouse_x, world_mouse_y;

    bool is_mouse_held;
    bool is_dragging;
    float drag_start_sx, drag_start_sy;
    float drag_curr_wx, drag_curr_wy;

    bool mouse_buttons[MAX_MOUSE_BTNS];
    bool mouse_just_pressed[MAX_MOUSE_BTNS];
    bool mouse_just_released[MAX_MOUSE_BTNS];
    int mouse_wheel;

    bool quit_requested;
} InputState;

void input_init(InputState* input);
void handle_events(InputState *input, bool *running);
void input_clear(InputState *input);
void input_update_world_coords(InputState *input, const Camera *cam);