#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define MAX_ENTITIES 512
#define MAX_KEYS 512
#define MAX_MOUSE_BTNS 5
#define DT (1.0 / 60.0)

typedef struct {float x, y;} Vec2;
typedef struct {int x, y;} Vec2_i;

#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__)

