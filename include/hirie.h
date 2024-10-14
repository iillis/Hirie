#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <stdint.h>
#include <string>

struct hirie_color {
    float r, g, b;
};

struct hirie_object;
struct hirie_window;

/*
MAIN
*/
hirie_window * hirie_init(uint64_t window_width, uint64_t window_height, uint64_t focal_lenght);
hirie_object * hirie_create_object(const std::string& filepath);
int hirie_project_object(struct hirie_object * model, hirie_color color, hirie_window* window);
int hirie_render_scene(hirie_window* window);
int hirie_clear_screen(hirie_window* window);
int hirie_exit(hirie_window* window);
/*
GEOMETRY
*/
void hirie_rot_x(float angle, hirie_object * model);
void hirie_rot_y(float angle, hirie_object * model);
void hirie_rot_z(float angle, hirie_object * model);
void hirie_move_x(float unit, hirie_object * model);
void hirie_move_y(float unit, hirie_object * model);
void hirie_move_z(float unit, hirie_object * model);
/*
INPUT
*/
int hirie_take_input(hirie_object * model);
int hirie_poll_exit(hirie_window* window);