#pragma once

#include <SDL2/SDL.h>

#include <hirie.hpp>

void project(float x, float y, float z, int *screenX, int *screenY, hirie_window * window);
void draw_line(float x1, float y1, float z1, float x2, float y2, float z2, hirie_window * window);