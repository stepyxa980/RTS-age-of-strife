#include "entity_pool.h"
#include "common.h"
#include <math.h>

#define SEP_DIST 24.0f
#define SEP_FORCE 100.0f

void pool_init(EntityPool *p) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        en->active = false;
        en->pos.x = en->pos.y = 0.0f;
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
    en->selected = false;
    en->is_moving = false;
    en->pos.x = x;
    en->pos.y = y;
    en->target.x = x;
    en->target.y = y;
    en->speed = 100.0f;
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
    (void)map_h; (void)map_w;
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        if (!en->active) continue;

        Vec2 move_dir = {0, 0};
        if (en->is_moving) {
            float dx = en->target.x - en->pos.x;
            float dy = en->target.y - en->pos.y;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist > 1.0f) {
                move_dir.x += (dx / dist) * en->speed;
                move_dir.y += (dy / dist) * en->speed;
            } else {
                en->is_moving = false;
            }
        }

        Vec2 separation = {0, 0};
        int neighbors = 0;
        for (int j = 0; j < MAX_ENTITIES; ++j) {
            if (i == j) continue;
            Entity *other = &p->entities[j];
            if (!other->active) continue;

            float dx = en->pos.x - other->pos.x;
            float dy = en->pos.y - other->pos.y;
            float dist_sq = dx*dx + dy*dy;

            if (dist_sq < (SEP_DIST * SEP_DIST) && dist_sq > 0.001f) {
                float dist = sqrtf(dist_sq);
                float force = (SEP_DIST - dist) / SEP_DIST;
                separation.x += (dx / dist) * force;
                separation.y += (dy / dist) * force;
                neighbors++;
            }
        }

        if (neighbors > 0) {
            separation.x *= SEP_FORCE;
            separation.y *= SEP_FORCE;

            move_dir.x += separation.x;
            move_dir.y += separation.y;
        }

        en->pos.x += move_dir.x * dt;
        en->pos.y += move_dir.y * dt;

        en->pos.x = CLAMP(en->pos.x, 0, map_w - en->size);
        en->pos.y = CLAMP(en->pos.y, 0, map_h - en->size);
    }
}

void pool_render(const EntityPool *p, SDL_Renderer *renderer, SDL_Texture *texture, const Camera *cam) {
    (void)texture;
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        const Entity* e = &p->entities[i];
        if (!e->active) continue;

        float sx, sy;
        camera_world_to_screen(cam, e->pos.x, e->pos.y, &sx, &sy);
        if (sx < -20 || sx > WINDOW_WIDTH + 20 || sy < -20 || sy > WINDOW_HEIGHT + 20) continue;
        
        float size = 16.0f * cam->zoom;
        SDL_Rect rect = {
            (int)(sx),
            (int)(sy),
            (int)(size),
            (int)(size)
        };

        if (e->selected) {
            SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, e->color.r, e->color.g, e->color.b, e->color.a);
        }
        SDL_RenderFillRect(renderer, &rect);
    }
}