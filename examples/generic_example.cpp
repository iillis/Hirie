#include "../include/hirie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("path to asset must be passed\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* assetPath = argv[1];  

    hirie_window* window = hirie_init(400, 400, 500);
    hirie_window* window2 = hirie_init(400, 400, 500);

    hirie_color white = {255, 255, 255};
    hirie_color red = {255, 0, 0};

    hirie_object* model = hirie_create_object(assetPath);
    hirie_object* model2 = hirie_create_object(assetPath);
    
    hirie_move_x(3, model2);

    while (!hirie_poll_exit(window) && !hirie_poll_exit(window2)) {
        hirie_project_object(model2, white, window);
        hirie_project_object(model, red, window2);
        hirie_rot_y(0.01, model2);
        hirie_take_input(model);
        hirie_render_scene(window);
        hirie_clear_screen(window);
        hirie_render_scene(window2);
        hirie_clear_screen(window2);
    }
    
    hirie_exit(window);
    hirie_exit(window2);
    return 0;
}