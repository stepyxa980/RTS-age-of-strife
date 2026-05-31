#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"
#include "camera.h"

typedef struct {
    Vec2 target;
    Vec2 pos;
    float speed;

    SDL_Color color;
    int uint_type;
    int size;

    bool active;
    bool is_moving;
    bool selected;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entity_count;
    int first_free;
}   EntityPool;

typedef struct {
    int tick_count;
    Camera camera;
    EntityPool pool;
} GameState;

void pool_init(EntityPool *p);
int pool_spawn(EntityPool *p, float x, float y, SDL_Color color);
void pool_despawn(EntityPool *p, int id);
void pool_update(EntityPool *p, float dt, float map_w, float map_h);
void pool_render(const EntityPool *p, SDL_Renderer *renderer, SDL_Texture *texture, const Camera *cam);