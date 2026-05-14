#pragma once

#include "entity_pool.h"
#include "common.h"
#include "input.h"

void spawn_grid(EntityPool* pool, int count, float spacing, float start_x, float start_y, SDL_Color col);
void game_init(GameState *state);
void game_update(GameState *state, const InputState *input, double dt, float screen_w, float screen_h, float world_w, float world_h);