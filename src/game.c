#include "game.h"
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
    state->camera = (Vec2){0.0f, 0.0f};
    state->map_wh = (Vec2){WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2};
    state->zoom = 1.0f;
    state->pool.entity_count = 0;

    pool_init(&state->pool);
    spawn_grid(&state->pool, 512, 14.0f, 20.0f, 20.0f, (SDL_Color){0, 200, 100, 255});
}

void game_update(GameState *state, const InputState *input, double dt) {
    if (!state || !input || dt <= 0.0) return;

    const float cam_speed = 300.0f * (float)dt;
    if (input->keys[SDL_SCANCODE_W]) state->camera.y -= cam_speed;
    if (input->keys[SDL_SCANCODE_S]) state->camera.y += cam_speed;
    if (input->keys[SDL_SCANCODE_A]) state->camera.x -= cam_speed;
    if (input->keys[SDL_SCANCODE_D]) state->camera.x += cam_speed;

    if (input->mouse_wheel != 0) {
        state->zoom += input->mouse_wheel * 0.1f;
        if (state->zoom < 0.5f) state->zoom = 0.5f;
        if (state->zoom > 3.0f) state->zoom = 3.0f;
    }

    if (input->mouse_just_pressed[2]) {
        SDL_Color color = {(uint8_t)(rand()%200+55), (uint8_t)(rand()%200+55), (uint8_t)(rand()%200+55), 255};

        float world_x = (input->mouse_x / state->zoom) + state->camera.x;
        float world_y = (input->mouse_y / state->zoom) + state->camera.y;

        pool_spawn(&state->pool, world_x - 8.0f, world_y - 8.0f, color);
    }

    if (input->key_just_pressed[SDL_SCANCODE_F]) {
        for (int i = 0; i < MAX_ENTITIES; ++i) {
            if (state->pool.entities[i].active) {
                pool_despawn(&state->pool, i);
                break;
            }
        }
    }

    pool_update(&state->pool, (float)dt, state->map_wh.x, state->map_wh.y);
}