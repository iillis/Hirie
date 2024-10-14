#include <SDL2/SDL.h>

#include "rendering.hpp"
#include <hirie.hpp>

void project(float x, float y, float z, int* screenX, int* screenY, hirie_window* window) {
    float projectedX = 0;
    float projectedY = 0;
    projectedX = (x / z) * window->focal_lenght;
    projectedY = (y / z) * window->focal_lenght;

    *screenX = (int)(window->width / 2 + projectedX);
    *screenY = (int)(window->height / 2 - projectedY);
}

void draw_line(float x1, float y1, float z1, float x2, float y2, float z2, hirie_window * window) {
    int screen_x1, screen_y1, screen_x2, screen_y2;

    bool point1InFront = (z1 > 0);
    bool point2InFront = (z2 > 0);

    if (point1InFront && point2InFront) {
        project(x1, y1, z1, &screen_x1, &screen_y1, window);
        project(x2, y2, z2, &screen_x2, &screen_y2, window);
        SDL_RenderDrawLine(window->renderer, screen_x1, screen_y1, screen_x2, screen_y2);
    } else if (point1InFront || point2InFront) {
        float xi, yi, t;
        if (!point1InFront) {
            t = z1 / (z1 - z2);
            xi = x1 + t * (x2 - x1);
            yi = y1 + t * (y2 - y1);
            project(xi, yi, 0.001f, &screen_x1, &screen_y1, window);
            project(x2, y2, z2, &screen_x2, &screen_y2, window);
            SDL_RenderDrawLine(window->renderer, screen_x1, screen_y1, screen_x2, screen_y2);
        } else {
            t = z2 / (z2 - z1);
            xi = x2 + t * (x1 - x2);
            yi = y2 + t * (y1 - y2);
            project(x1, y1, z1, &screen_x1, &screen_y1, window);
            project(xi, yi, 0.001f, &screen_x2, &screen_y2, window);
            SDL_RenderDrawLine(window->renderer, screen_x1, screen_y1, screen_x2, screen_y2);
        }
    }
}