#pragma once

#include "game.h"
#include "input.h"
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *entity_texture;
} RenderContext;

bool render_init(RenderContext* ctx);
void render_cleanup(RenderContext* ctx);
void render_draw(RenderContext* ctx, const GameState* state, const InputState* input, double interpolation);