#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define MAP_W 1000
#define MAP_H 800
#define MAX_ENTITIES 512
#define MAX_KEYS 512
#define MAX_MOUSE_BTNS 5
#define DT (1.0 / 60.0)
#define DRAG_THRESHOLD 5.0f

typedef struct {float x, y;} Vec2;
typedef struct {int x, y;} Vec2_i;

#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) fprintf(stderr, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) fprintf(stderr, "[ERR]  " fmt "\n", ##__VA_ARGS__)

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define CLAMP(v, min, max) (MIN(MAX((v), (min)), (max)))