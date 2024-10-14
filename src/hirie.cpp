#include <SDL2/SDL.h>
#include <math.h>
#include <numbers>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "hirie.hpp"
#include <input/input.hpp>
#include <parsing/parsing.hpp>
#include <geometry/matrices.hpp>
#include <rendering/rendering.hpp>

hirie_window* hirie_init(uint64_t window_width, uint64_t window_height, uint64_t focal_lenght) {
    struct hirie_window* window = new hirie_window;
    window->width = window_width;
    window->height = window_height;
    window->focal_lenght = focal_lenght;
    SDL_Init(SDL_INIT_EVERYTHING);
    window->window = SDL_CreateWindow("Hirie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_ALLOW_HIGHDPI);
    window->renderer = SDL_CreateRenderer(window->window, -1, 0);
    SDL_RenderSetLogicalSize(window->renderer, window_width, window_height);

    keys = SDL_GetKeyboardState(NULL);
    return window;
}

hirie_object* hirie_create_object(const std::string& filepath) {
    struct hirie_object* model = new hirie_object(0.0f, 0.0f, 5.0f);
    if(hirie_parse_obj(model->vertices, model->faces, filepath)) return NULL;
    return model;
}

int hirie_project_object(struct hirie_object* model, hirie_color color, hirie_window* window) {
    SDL_SetRenderDrawColor(window->renderer, color.r, color.b, color.g, 255);
    for(hirie_vertex vert : model->vertices) {
        vert.last_drawn = 0;
    }
    hirie_vertex start, old;
    hirie_vertex* older = NULL,* starter = NULL;
    for(const auto& face : model->faces) { 
        bool first = true;
        for(int i = 0; i < face.vertex_indices.size(); i++ ) {
            int vi = face.vertex_indices[i] - 1;

            hirie_vertex vert = model->vertices[vi];

            float x = vert.x + model->origin.x,
                  y = vert.y + model->origin.y,
                  z = vert.z + model->origin.z;

            if(first) {
                old.x = x;
                old.y = y;
                old.z = z;
                start.x = x;
                start.y = y;
                start.z = old.z;
                older = &model->vertices[vi];
                starter = &model->vertices[vi];
                first = false;
                continue;
            }
            if(i == face.vertex_indices.size()-1 && starter->last_drawn != vi) {
                draw_line(x, y, z, start.x, start.y, start.z, window);
                starter->last_drawn = vi;
            }
            if(older->last_drawn != vi) {
                draw_line(old.x, old.y, old.z, x, y, z, window);
                older->last_drawn = vi;
            }
            older = &model->vertices[vi];
            old.x = x;
            old.y = y;
            old.z = z;
        }
    }
    return 0;
}

int hirie_render_scene(hirie_window* window) {
    SDL_RenderPresent(window->renderer);
    return 0;
}

int hirie_clear_screen(hirie_window* window) {
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    return SDL_RenderClear(window->renderer);
}

int hirie_exit(hirie_window* window) {
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return 0;
}