#include <hirie.hpp>
#include "matrices.hpp"

#include <math.h>

void hirie_rot_x(float angle, hirie_object * model) {
    for(int i = 0; i < model->vertices.size(); ++i) {
        float y = model->vertices[i].y, z = model->vertices[i].z;
        model->vertices[i].y = y*cos(angle) - z*sin(angle);
        model->vertices[i].z = y*sin(angle) + z*cos(angle);
    }
}

void hirie_rot_y(float angle, hirie_object * model) {
    for(int i = 0; i < model->vertices.size(); ++i) {
        float x = model->vertices[i].x, z = model->vertices[i].z;
        model->vertices[i].x = x*cos(angle) + z*sin(angle);
        model->vertices[i].z = -x*sin(angle) + z*cos(angle);
    }
}

void hirie_rot_z(float angle, hirie_object * model) {
    for(int i = 0; i < model->vertices.size(); ++i) {
        float x = model->vertices[i].x, y = model->vertices[i].y;
        model->vertices[i].x = x*cos(angle) - y*sin(angle);
        model->vertices[i].y = x*sin(angle) + y*cos(angle);
    }
}

void hirie_move_x(float unit, hirie_object * model) {
    model->origin.x+=unit;
}

void hirie_move_y(float unit, hirie_object * model) {
    model->origin.y+=unit;
}

void hirie_move_z(float unit, hirie_object * model) {
    model->origin.z+=unit;
}