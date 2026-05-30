#include "camera.h"

void camera_init(Camera *cam) {
    cam->pos.x = 0.0f; cam->pos.y = 0.0f;
    cam->zoom = 1.0f;
    cam->min_zoom = 0.5f, cam->max_zoom = 3.0f;
    cam->pan_speed = 400.0f;
}

void camera_pan(Camera *cam, float dx, float dy, float dt, float screen_w, float screen_h, float world_w, float world_h) {
    cam->pos.x += dx * cam->pan_speed * dt;
    cam->pos.y += dy * cam->pan_speed * dt;

    float max_x = world_w - screen_w / cam->zoom;
    float max_y = world_h - screen_h / cam->zoom;

    cam->pos.x = CLAMP(cam->pos.x, 0.0f, max_x);
    cam->pos.y = CLAMP(cam->pos.y, 0.0f, max_y);
}

void camera_zoom(Camera *cam, int wheel_delta) {
    if (wheel_delta == 0) return;
    cam->zoom += wheel_delta * 0.1f;
    cam->zoom = CLAMP(cam->zoom, cam->min_zoom, cam->max_zoom);
}

void camera_screen_to_world(const Camera *cam, float sx, float sy, float *out_wx, float *out_wy) {
    *out_wx = (sx / cam->zoom) + cam->pos.x;
    *out_wy = (sy / cam->zoom) + cam->pos.y;
}

void camera_world_to_screen(const Camera *cam, float wx, float wy, float *out_sx, float *out_sy) {
    *out_sx = (wx - cam->pos.x) * cam->zoom;
    *out_sy = (wy - cam->pos.y) * cam->zoom;
}