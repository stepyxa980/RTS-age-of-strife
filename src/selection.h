#pragma once
#include "entity_pool.h"

bool point_in_rect(float px, float py, float rx1, float ry1, float rx2, float ry2);
void selection_click(EntityPool *pool, float wx, float wy);
void selection_drag(EntityPool *pool, float sx, float sy, float ex, float ey);