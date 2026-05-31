#include "input.h"
#include <string.h>

void input_init(InputState *input) {
    memset(input, 0, sizeof(InputState));
}

void handle_events(InputState *input, bool *running) {
    if (!input) return;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = false;
                input->quit_requested = true;
                break;
            
            case SDL_KEYDOWN:
                if (event.key.repeat > 0) break;
                if (event.key.keysym.scancode < MAX_KEYS) {
                    input->keys[event.key.keysym.scancode] = true;
                    input->key_just_pressed[event.key.keysym.scancode] = true;
                }
                break;
            
            case SDL_KEYUP:
                if (event.key.keysym.scancode < MAX_KEYS) {
                    input->keys[event.key.keysym.scancode] = false;
                    input->key_just_released[event.key.keysym.scancode] = true;
                }
                break;
            
            case SDL_MOUSEMOTION:
                input->mouse_x = (float)event.motion.x;
                input->mouse_y = (float)event.motion.y;
                if (input->is_mouse_held) {
                    float dx = input->mouse_x - input->drag_start_sx;
                    float dy = input->mouse_y - input->drag_start_sy;
                    if (hypotf(dx, dy) > DRAG_THRESHOLD) input ->is_dragging = true;
                }
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button >= 1 && event.button.button <= MAX_MOUSE_BTNS) {
                    input->mouse_buttons[event.button.button - 1] = true;
                    input->mouse_just_pressed[event.button.button - 1] = true;
                }
                if (event.button.button == 1) {
                    input->is_mouse_held = true;
                    input->is_dragging = false;
                    input->drag_start_sx = (float)event.button.x;
                    input->drag_start_sy = (float)event.button.y;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button >= 1 && event.button.button <= MAX_MOUSE_BTNS) {
                    input->mouse_buttons[event.button.button - 1] = false;
                    input->mouse_just_released[event.button.button - 1] = true;
                    if (event.button.button == 1) input->is_mouse_held = false;
                }
                break;
            
            case SDL_MOUSEWHEEL:
                input->mouse_wheel += event.wheel.y;
                break;
        }
    }
}

void input_clear(InputState* input) {
    if (!input) return;
    memset(input->key_just_pressed, 0, sizeof(input->key_just_pressed));
    memset(input->key_just_released, 0, sizeof(input->key_just_released));
    memset(input->mouse_just_pressed, 0, sizeof(input->mouse_just_pressed));
    memset(input->mouse_just_released, 0, sizeof(input->mouse_just_released));
    input->mouse_wheel = 0;
}

void input_update_world_coords(InputState *input, const Camera *cam) {
    camera_screen_to_world(cam, input->mouse_x, input->mouse_y, &input->world_mouse_x, &input->world_mouse_y);
    input->drag_curr_wx = input->world_mouse_x;
    input->drag_curr_wy = input->world_mouse_y;
}