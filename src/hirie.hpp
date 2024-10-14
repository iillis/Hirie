#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <stdint.h>

#define DEFAULT_ANGLE   (M_PI/180)

struct hirie_color {
    float r, g, b;
};

struct hirie_vertex {
    float x, y, z;
    int last_drawn;
};

struct hirie_face {
    std::vector<int> vertex_indices;
};

struct hirie_object {
    hirie_vertex origin;
    std::vector<hirie_vertex> vertices;
    std::vector<hirie_face> faces;
    hirie_object(float x, float y, float z) : origin{x, y, z} {};
};

struct hirie_window {
    uint64_t width, height, focal_lenght;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
};