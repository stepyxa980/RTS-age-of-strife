#include "entity_pool.h"
#include "common.h"

void pool_init(EntityPool *p) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        en->active = false;
        en->id = -1;
        en->pos.x = en->pos.y = 0.0f;
        en->vel.x = en->vel.y = 0.0f;
        en->speed = 0.0f;
        en->size = 16;
        en->color = (SDL_Color){255, 255, 255, 255};
    }
    p->entity_count = 0;
    p->first_free = 0;
}

int pool_spawn(EntityPool *p, float x, float y, SDL_Color color) {
    if (p->first_free < 0 || p->first_free >= MAX_ENTITIES) {
        LOG_INFO("Entity pool full!");
        return -1;
    }
    int idx = p->first_free;
    Entity *en = &p->entities[idx];
    en->active = true;
    en->id = idx;
    en->pos.x = x;
    en->pos.y = y;
    en->vel.x = 300.0f;
    en->vel.y = 300.0f * (idx % 2 ? 1 : -1);
    en->color = color;
    p->entity_count++;
    p->first_free = idx + 1;
    while (p->first_free < MAX_ENTITIES && p->entities[p->first_free].active) {
        p->first_free++;
    }
    if (p->first_free >= MAX_ENTITIES) {
        p->first_free = -1;
    }
    return idx;
}

void pool_despawn(EntityPool *p, int id) {
    if (id >= 0 && id < MAX_ENTITIES && p->entities[id].active) {
        p->entities[id].active = false;
        p->entity_count--;
        if (p->first_free < 0 || id < p->first_free) {
            p->first_free = id;
        }
    }
}

void pool_update(EntityPool *p, float dt, float map_w, float map_h) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        if (!en->active) continue;

        en->pos.x += en->vel.x * dt;
        en->pos.y += en->vel.y * dt;

        float size = (float)en->size;
        if (en->pos.x <= 0.0 || en->pos.x >= map_w - size) en->vel.x *= -1.0f;
        if (en->pos.y <= 0.0 || en->pos.y >= map_h - size) en->vel.y *= -1.0f;
    }
}

void pool_render(EntityPool *p, GameState *state, SDL_Renderer *renderer, SDL_Texture *texture) {
    (void)texture;
    const float zoom = state->zoom;

    for (int i = 0; i < MAX_ENTITIES; ++i) {
        const Entity* e = &p->entities[i];
        if (!e->active) continue;

        SDL_Rect rect = {
            (int)((e->pos.x - state->camera.x) * zoom),
            (int)((e->pos.y - state->camera.y) * zoom),
            (int)(e->size * zoom),
            (int)(e->size * zoom)
        };

        if (rect.x + rect.w < 0 || rect.y + rect.h < 0 ||
            rect.x > WINDOW_WIDTH || rect.y > WINDOW_HEIGHT) continue;

        SDL_SetRenderDrawColor(renderer, e->color.r, e->color.g, e->color.b, e->color.a);
        SDL_RenderFillRect(renderer, &rect);

        if (e->selected) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}