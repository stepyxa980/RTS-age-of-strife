#include "game.h"
#include "selection.h"
#include <stdio.h>
#include <stdlib.h>

void spawn_grid(EntityPool* pool, int count, float spacing, float start_x, float start_y, SDL_Color color) {
    int cols = (int)ceilf(sqrtf((float)count));
    int spawned = 0;

    for (int row = 0; row < cols && spawned < count; ++row) {
        for (int c = 0; c < cols && spawned < count; ++c) { // ← заменил col на c
            float x = start_x + c * spacing;
            float y = start_y + row * spacing;
            pool_spawn(pool, x, y, color);
            spawned++;
        }
    }
}

void game_init(GameState *state) {
    if (!state) return;
    memset(state, 0, sizeof(GameState));
    camera_init(&state->camera);

    pool_init(&state->pool);
    state->tick_count = 0;
    spawn_grid(&state->pool, 32, 14.0f, 20.0f, 20.0f, (SDL_Color){0, 200, 100, 255});
}

void game_update(GameState *state, const InputState *input, double dt, float screen_w, float screen_h, float world_w, float world_h) {
    if (!state || !input || dt <= 0.0) return;

    float dx = 0.0f, dy = 0.0f;
    if (input->keys[SDL_SCANCODE_W]) dy = -1.0f;
    if (input->keys[SDL_SCANCODE_S]) dy = 1.0f;
    if (input->keys[SDL_SCANCODE_A]) dx = -1.0f;
    if (input->keys[SDL_SCANCODE_D]) dx = 1.0f;

    camera_pan(&state->camera, dx, dy, (float)dt, screen_w, screen_h, world_w, world_h);

    if (input->mouse_wheel != 0) {
        camera_zoom(&state->camera, input->mouse_wheel);
    }

    if (input->mouse_just_pressed[1]) {
        pool_spawn(&state->pool, input->world_mouse_x - 8.0f, input->world_mouse_y - 8.0f, (SDL_Color){0, 100, 200, 255});
    }

    if (input->key_just_pressed[SDL_SCANCODE_F]) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (state->pool.entities[i].active) {
                pool_despawn(&state->pool, i);
                break;
            }
        }
    }

    if (input->mouse_just_pressed[2]) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            Entity *en = &state->pool.entities[i];
            if (en->active && en->selected) {
                en->target.x = input->world_mouse_x;
                en->target.y = input->world_mouse_y;
                en->is_moving = true;
            }
        }
    }

    if (input->mouse_just_pressed[0]) {
        selection_click(&state->pool, input->world_mouse_x, input->world_mouse_y);
    }

    if (input->mouse_just_released[0] && input->is_dragging) {
        float start_wx, start_wy;
        camera_screen_to_world(&state->camera, input->drag_start_sx, input->drag_start_sy, &start_wx, &start_wy);
        selection_drag(&state->pool, start_wx, start_wy, input->drag_curr_wx, input->drag_curr_wy);
    }

    if (input->key_just_pressed[SDL_SCANCODE_SPACE]) {
        for (int i = 0; i < MAX_ENTITIES; i++) state->pool.entities[i].selected = false;
    }

    pool_update(&state->pool, (float)dt, world_w, world_h);
    state->tick_count++;
}