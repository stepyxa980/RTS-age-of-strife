#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"

typedef struct {
    bool active;
    bool selected;
    int id;
    Vec2 pos;
    Vec2 vel;
    float speed;
    int size;
    float target_x, target_y;
    uint8_t uint_type;
    SDL_Color color;
} Entity;

typedef struct {
    Entity entities[MAX_ENTITIES];
    int entity_count;
    int first_free;
}   EntityPool;

typedef struct {
    EntityPool pool; 
    Vec2 camera;
    float zoom;
    Vec2 map_wh;
} GameState;

void pool_init(EntityPool *p);
int pool_spawn(EntityPool *p, float x, float y, SDL_Color color);
void pool_despawn(EntityPool *p, int id);
void pool_update(EntityPool *p, float dt, float map_w, float map_h);
void pool_render(EntityPool *p, GameState *state, SDL_Renderer *renderer);