#pragma once

#include "game.h"
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} RenderContext;

bool render_init(RenderContext* ctx);
void render_cleanup(RenderContext* ctx);
void render_draw(const RenderContext* ctx, const GameState* state, double interpolation);