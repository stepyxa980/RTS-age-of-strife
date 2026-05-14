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

    ctx->entity_texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC, 16, 16);
    if (!ctx->entity_texture) {
        LOG_ERR("SDL_CreateTexture: %s\n", SDL_GetError());
        return false;
    }
    uint32_t pixels[16 * 16];
    memset(pixels, 0xFF, sizeof(pixels));
    SDL_UpdateTexture(ctx->entity_texture, NULL, pixels, 16 * sizeof(uint32_t));

    return true;
}

void render_cleanup(RenderContext* ctx) {
    if (!ctx) return;
    if (ctx->entity_texture) SDL_DestroyTexture(ctx->entity_texture);
    if (ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
    if (ctx->window) SDL_DestroyWindow(ctx->window);
}

void render_draw(RenderContext *ctx, const GameState *state, const InputState *input, double interpolation) {
    if (!ctx || !state) return;
    (void)interpolation;

    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->renderer);

    pool_render((EntityPool*)&state->pool, ctx->renderer, ctx->entity_texture, &state->camera);

    float sx1, sy1, sx2, sy2;
    canera_world_to_screen(&state->camera, 0, 0, &sx1, &sy1);
    canera_world_to_screen(&state->camera, MAP_W, MAP_H, &sx2, &sy2);
    SDL_Rect border = {(int)sx1, (int)sy1, (int)(sx2 - sx1), (int)(sy2 - sy1)};
    SDL_SetRenderDrawColor(ctx->renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(ctx->renderer, &border);

    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 200);
    SDL_Rect cursor = {(int)input->mouse_x - 2, (int)input->mouse_y - 2, 4, 4};
    SDL_RenderFillRect(ctx->renderer, &cursor);

    SDL_RenderPresent(ctx->renderer);
}