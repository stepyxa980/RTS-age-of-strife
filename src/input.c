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
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button >= 1 && event.button.button <= MAX_MOUSE_BTNS) {
                    input->mouse_buttons[event.button.button - 1] = true;
                    input->mouse_just_pressed[event.button.button - 1] = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button >= 1 && event.button.button <= MAX_MOUSE_BTNS) {
                    input->mouse_buttons[event.button.button - 1] = false;
                    input->mouse_just_released[event.button.button - 1] = true;
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