#include <stdint.h>
#include <SDL2/SDL.h>

#include "input.hpp"
#include <hirie.hpp>
#include <geometry/matrices.hpp>

const uint8_t *keys;

int hirie_take_input(hirie_object * model) {
    if (keys[SDL_SCANCODE_W]) { hirie_rot_x(DEFAULT_ANGLE, model); return true; }
    else if (keys[SDL_SCANCODE_S]) { hirie_rot_x(-DEFAULT_ANGLE, model); return true; }
    if (keys[SDL_SCANCODE_A]) { hirie_rot_y(-DEFAULT_ANGLE, model); return true; }
    else if (keys[SDL_SCANCODE_D]) { hirie_rot_y(DEFAULT_ANGLE, model); return true; }
    if (keys[SDL_SCANCODE_Q]) { hirie_rot_z(-DEFAULT_ANGLE, model); return true; }
    else if (keys[SDL_SCANCODE_E]) { hirie_rot_z(DEFAULT_ANGLE, model); return true; }
    if (keys[SDL_SCANCODE_UP]) { hirie_move_z(0.1f, model); return true; }
    else if (keys[SDL_SCANCODE_DOWN]) { hirie_move_z(-0.1f, model); return true; }
    if (keys[SDL_SCANCODE_I]) { hirie_move_y(-0.1f, model); return true; }
    else if (keys[SDL_SCANCODE_K]) {hirie_move_y(0.1f, model); return true; }
    if (keys[SDL_SCANCODE_J]) { hirie_move_x(-0.1f, model); return true; }
    else if (keys[SDL_SCANCODE_L]) { hirie_move_x(0.1f, model); return true; }
    return 1;
}

int hirie_poll_exit(hirie_window* window) {
    static SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return -1;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            return event.window.windowID;
        }
    }
    return 0;
}
