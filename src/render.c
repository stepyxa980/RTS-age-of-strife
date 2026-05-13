// render.c
#include "render.h"
#include <stdio.h>

bool render_init(RenderContext* ctx) {
    if (!ctx) return false;
    ctx->window = SDL_CreateWindow("RTS Phase 2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!ctx->window) {
        LOG_ERR("SDL_CreateWindow: %s\n", SDL_GetError());
        return false;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
    if (!ctx->renderer) {
        LOG_ERR("SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        return false;
    }

    SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_BLEND);
    return true;
}

void render_cleanup(RenderContext* ctx) {
    if (!ctx) return;
    if (ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
    if (ctx->window) SDL_DestroyWindow(ctx->window);
}

void render_draw(const RenderContext *ctx, const GameState *state, double interpolation) {
    if (!ctx || !state) return;
    (void)interpolation;

    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->renderer);

    pool_render((EntityPool*)&state->pool, (GameState*)state, ctx->renderer);

    SDL_SetRenderDrawColor(ctx->renderer, 80, 80, 80, 255);
    SDL_Rect border = {0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderDrawRect(ctx->renderer, &border);

    SDL_RenderPresent(ctx->renderer);
}