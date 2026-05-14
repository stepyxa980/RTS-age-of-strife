#pragma once
#include "common.h"

typedef struct {
    Vec2 pos;
    float zoom;
    float min_zoom;
    float max_zoom;
    float pan_speed;
} Camera;

void camera_init(Camera *cam);
void camera_pan(Camera *cam, float dx, float dy, float dt, float screen_w, float screen_h, float world_w, float world_h);
void camera_zoom(Camera *cam, int wheel_delta);
void camera_screen_to_world(const Camera *cam, float sx, float sy, float *out_wx, float *out_wy);
void canera_world_to_screen(const Camera *cam, float wx, float wy, float *out_sx, float *out_sy);