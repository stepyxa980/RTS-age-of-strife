#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PROF_SAMPLES 60

static uint64_t g_prof_freq = 0;
static double g_upd_prof[PROF_SAMPLES] = {0};
static double g_rnd_prof[PROF_SAMPLES] = {0};
static int g_prof_idx = 0;

static inline void prof_init(void) {g_prof_freq = SDL_GetPerformanceFrequency();}

static inline double prof_ms(uint64_t ticks) {
    return (double)ticks * 1000.0 / (double)g_prof_freq;
}

static inline void prof_start(uint64_t *out) {*out = SDL_GetPerformanceCounter();}

static inline void prof_end(uint64_t start, bool is_update) {
    double ms = prof_ms(SDL_GetPerformanceCounter() - start);
    if (g_prof_idx < PROF_SAMPLES) {
        if (is_update) g_upd_prof[g_prof_idx] = ms;
        else g_rnd_prof[g_prof_idx] = ms;
        g_prof_idx++;
    }
}

static inline void prof_log(void) {
    if (g_prof_idx == 0) return;
    double u = 0, r = 0;
    for (int i = 0; i < g_prof_idx; i++) {
        u += g_upd_prof[i];
        r += g_rnd_prof[i];
    }
    printf("[PROF] Avg Update: %.2fms | Render: %.2fms | Samples: %d\n", 
        u/g_prof_idx, r/g_prof_idx, g_prof_idx);
    g_prof_idx = 0;
}

static inline void profiler_log_fps(double frame_time_ms) {
    static double fps_acc = 0.0;
    static int fps_frames = 0;
    fps_acc += frame_time_ms;
    fps_frames++;
    double fps = (double)fps_frames / fps_acc;
    printf("FPS: %.1f | ", fps);
    fps_acc = 0.0;
    fps_frames = 0;
}