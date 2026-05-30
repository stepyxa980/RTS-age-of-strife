#include <SDL2/SDL.h>
#include <stdio.h>
#include "common.h"
#include "input.h"
#include "game.h"
#include "render.h"
#include "profiler.h"

int main(int argc, char *args[]) {
    (void) argc; (void) args;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERR("SDL_Init failed %s\n", SDL_GetError());
        return 1;
    }

    static GameState state = {0};
    static InputState input = {0};
    static RenderContext render_ctx = {0};

    game_init(&state);
    input_init(&input);
    if (!render_init(&render_ctx)) {
        LOG_ERR("Render failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    prof_init();
    static uint64_t last_log_time = 0;

    double accumulator = 0.0;
    uint64_t last = SDL_GetPerformanceCounter();
    bool running = true;
    uint64_t t_udp;
    uint64_t t_rnd;

    while(running) {
        Uint64 now = SDL_GetPerformanceCounter();
        double frame_time = (double)(now - last) / (double)SDL_GetPerformanceFrequency();
        last = now;
        if (frame_time > 0.25) frame_time = 0.25;

        accumulator += frame_time;

        handle_events(&input, &running);
        input_update_world_coords(&input, &state.camera);

        prof_start(&t_udp);
        while (accumulator >= DT) {
            game_update(&state, &input, DT, WINDOW_WIDTH, WINDOW_HEIGHT, MAP_W, MAP_H);
            input_clear(&input);
            accumulator -= DT;
        }
        prof_end(t_udp, true);

        double interp = accumulator / DT;
        prof_start(&t_rnd);
        render_draw(&render_ctx, &state, &input, interp); // &&&&&
        prof_end(t_rnd, false);

        uint64_t current_time = SDL_GetPerformanceCounter();
        if (current_time - last_log_time >= g_prof_freq) {
            prof_log();
            profiler_log_fps(frame_time);
            last_log_time = current_time;
        }
    }

    render_cleanup(&render_ctx);
    SDL_Quit();
    return 0;
}