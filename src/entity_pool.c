#include "entity_pool.h"
#include "common.h"
#include <math.h>

#define SEP_DIST 24.0f
#define SEP_FORCE 100.0f
#define FATIGUE_GAIN 15.0f
#define FATIGUE_RECOVERY 10.0f
#define SPACING 24.0f

void pool_init(EntityPool *p) {
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        en->active = false;
        en->selected = false;
        en->is_moving = false;
        en->pos.x = en->pos.y = 0.0f;
        en->speed = 0.0f;
        en->size = 16;
        en->color = (SDL_Color){255, 255, 255, 255};
        en->fatigue = 0.0f;
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
    en->size = 16;
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

void pool_move_selected(EntityPool *p, float target_x, float target_y) {
    int selected_count = 0;
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        if (p->entities[i].active && p->entities[i].selected) {
            selected_count++;
        }
    }
    if (selected_count == 0) return;

    int cols = (int)ceilf(sqrtf((float)selected_count));
    int idx = 0;
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        if (en->active && en->selected) {
            int row = idx / cols;
            int col = idx % cols;
            en->target.x = target_x + (col - (cols - 1) * 0.5f) * SPACING;
            en->target.y = target_y + (row - (cols - 1) * 0.5f) * SPACING;
            en->is_moving = true;
            idx++;
        }
    }
}

void pool_update(EntityPool *p, float dt, float map_w, float map_h) {
    const float SEP_DIST_SQ = SEP_DIST * SEP_DIST;
    const float ARRIVE_THRESH_SQ = 1.0f; 

    for (int i = 0; i < MAX_ENTITIES; ++i) {
        Entity *en = &p->entities[i];
        if (!en->active) continue;

        Vec2 velocity = {0.0f, 0.0f};

        if (en->is_moving) {
            float dx = en->target.x - en->pos.x;
            float dy = en->target.y - en->pos.y;
            float dist_sq = dx*dx + dy*dy;

            if (dist_sq > ARRIVE_THRESH_SQ) {
                en->fatigue += FATIGUE_GAIN * dt;
                if (en->fatigue > 100.0f) en->fatigue = 100.0f;

                float current_speed = en->speed * (1.0f - (en->fatigue / 150.0f));

                float inv_dist = 1.0f / sqrtf(dist_sq);
                velocity.x = dx * inv_dist * current_speed;
                velocity.y = dy * inv_dist * current_speed;

            } else {
                en->is_moving = false;
            }
        } else {
            en->fatigue -= FATIGUE_RECOVERY * dt;
            if (en->fatigue < 0.0f) en->fatigue = 0.0f;
        }

        Vec2 separation = {0.0f, 0.0f};
        for (int j = 0; j < MAX_ENTITIES; ++j) {
            if (i == j) continue;
            Entity *other = &p->entities[j];
            if (!other->active) continue;

            float dx = en->pos.x - other->pos.x;
            float dy = en->pos.y - other->pos.y;
            float dist_sq = dx*dx + dy*dy;

            if (dist_sq < SEP_DIST_SQ && dist_sq > 0.0001f) {
                float dist = sqrtf(dist_sq);
                float force = (SEP_DIST - dist) / SEP_DIST;
                
                float inv_dist = 1.0f / dist;
                separation.x += dx * inv_dist * force * SEP_FORCE;
                separation.y += dy * inv_dist * force * SEP_FORCE;
            }
        }

        en->pos.x += (velocity.x + separation.x) * dt;
        en->pos.y += (velocity.y + separation.y) * dt;

        en->pos.x = CLAMP(en->pos.x, 0.0f, map_w - (float)en->size);
        en->pos.y = CLAMP(en->pos.y, 0.0f, map_h - (float)en->size);
    }
}

void pool_render(const EntityPool *p, SDL_Renderer *renderer, SDL_Texture *texture, const Camera *cam) {
    (void)texture;
    for (int i = 0; i < MAX_ENTITIES; ++i) {
        const Entity* en = &p->entities[i];
        if (!en->active) continue;

        float sx, sy;
        camera_world_to_screen(cam, en->pos.x, en->pos.y, &sx, &sy);
        if (sx < -20 || sx > WINDOW_WIDTH + 20 || sy < -20 || sy > WINDOW_HEIGHT + 20) continue;
        
        float size = 16.0f * cam->zoom;
        SDL_Rect rect = {
            (int)(sx),
            (int)(sy),
            (int)(size),
            (int)(size)
        };

        if (en->selected) {
            SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255);
        } else {
            float t = en->fatigue / 100.0f;
            SDL_SetRenderDrawColor(renderer,
                (Uint8)(en->color.r * (1.0f - t) + 255 * t), // R растет до 255
                (Uint8)(en->color.g * (1.0f - t)),           // G падает до 0
                (Uint8)(en->color.b * (1.0f - t)),           // B падает до 0
                en->color.a);
        }
        SDL_RenderFillRect(renderer, &rect);
    }
}