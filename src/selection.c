#include "selection.h"

bool point_in_rect(float px, float py, float rx1, float ry1, float rx2, float ry2) {
    return px >= rx1 && px <= rx2 && py >= ry1 && py <= ry2;
}

void selection_click(EntityPool *pool, float wx, float wy) {
    for (int i = 0; i < MAX_ENTITIES; i++) pool->entities[i].selected = false;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!pool->entities[i].active) continue;
        float cx = pool->entities[i].pos.x + 8.0f;
        float cy = pool->entities[i].pos.y + 8.0f;
        if (fabsf(cx - wx) <= 8.0f && fabsf(cy - wy) <= 8.0f) {
            pool->entities[i].selected = true;
            break;
        }
    }
}

void selection_drag(EntityPool *pool, float sx, float sy, float ex, float ey) {
    float rx1 = fminf(sx, ex), ry1 = fminf(sy, ey);
    float rx2 = fmaxf(sx, ex), ry2 = fmaxf(sy, ey);

    for (int i = 0; i < MAX_ENTITIES; i++) {
        pool->entities[i].selected = false;
        if (!pool->entities[i].active) continue;
        float cx = pool->entities[i].pos.x + 8.0f;
        float cy = pool->entities[i].pos.y + 8.0f;
        if (point_in_rect(cx, cy, rx1, ry1, rx2, ry2)) {
            pool->entities[i].selected = true;
        }
    }
}