#include <SDL2/SDL.h>
#include <math.h>
#include <numbers>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>
#include <chrono>
#include <thread>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const float FOCAL_LENGTH = 220.0f;

# define PI 3.141592653589793238462643383279502884L

#define DEFAULT_ANGLE (PI/180)

typedef struct {
    float x, y;
} point;

struct vertex {
    float x, y, z;
};

struct face {
    std::vector<int> vertex_indices;
};

struct object {
    vertex origin;
} model;

std::vector<vertex> vertices;
std::vector<face> faces;

bool moving = true;
const Uint8 *keys;
SDL_Event event;

void parse_face(const std::string& line, face& face) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;
    while (iss >> token) {
        std::istringstream element(token);
        std::string v;
        std::getline(element, v, '/');        
        if (!v.empty()) {
            face.vertex_indices.push_back(std::stoi(v));
        }
    }
}

void rot_x(float angle){
    for (int i = 0; i < vertices.size(); ++i) {
        float x = vertices[i].x, y = vertices[i].y, z = vertices[i].z;
        vertices[i].y = y*cos(angle) - z*sin(angle);
        vertices[i].z = y*sin(angle) + z*cos(angle);
    }
}

void rot_y(float angle){
    for (int i = 0; i < vertices.size(); ++i) {
        float x = vertices[i].x, y = vertices[i].y, z = vertices[i].z;
        vertices[i].x = x*cos(angle) + z*sin(angle);
        vertices[i].z = -x*sin(angle) + z*cos(angle);
    }
}

void rot_z(float angle){
    for (int i = 0; i < vertices.size(); ++i) {
        float x = vertices[i].x, y = vertices[i].y, z = vertices[i].z;
        vertices[i].x = x*cos(angle) - y*sin(angle);
        vertices[i].y = x*sin(angle) + y*cos(angle);
    }
}

void move_x(float unit){
    model.origin.x+=unit;
}
void move_y(float unit){
    model.origin.y+=unit;
}

void take_input(void) {
    if (keys[SDL_SCANCODE_W]) { rot_x(DEFAULT_ANGLE); moving = true; }
    else if (keys[SDL_SCANCODE_S]) { rot_x(-DEFAULT_ANGLE); moving = true; }
    if (keys[SDL_SCANCODE_A]) { rot_y(-DEFAULT_ANGLE); moving = true; }
    else if (keys[SDL_SCANCODE_D]) { rot_y(DEFAULT_ANGLE); moving = true; }
    if (keys[SDL_SCANCODE_Q]) { rot_z(-DEFAULT_ANGLE); moving = true; }
    else if (keys[SDL_SCANCODE_E]) { rot_z(DEFAULT_ANGLE); moving = true; }

    if (keys[SDL_SCANCODE_UP]) { model.origin.z += 0.1f; moving = true; }
    else if (keys[SDL_SCANCODE_DOWN]) { model.origin.z -= 0.1f; moving = true; }
    if (keys[SDL_SCANCODE_I]) { model.origin.y -= 0.1f; moving = true; }
    else if (keys[SDL_SCANCODE_K]) { model.origin.y += 0.1f; moving = true; }
    if (keys[SDL_SCANCODE_J]) { model.origin.x -= 0.1f; moving = true; }
    else if (keys[SDL_SCANCODE_L]) { model.origin.x += 0.1f; moving = true; }
}

void project(float x, float y, float z, int *screenX, int *screenY) {
    float projectedX = 0;
    float projectedY = 0;
    projectedX = (x / z) * FOCAL_LENGTH;
    projectedY = (y / z) * FOCAL_LENGTH;

    *screenX = (int)(SCREEN_WIDTH / 2 + projectedX);
    *screenY = (int)(SCREEN_HEIGHT / 2 - projectedY);
}

void draw_line(float x1, float y1, float z1, float x2, float y2, float z2, SDL_Renderer* renderer) {
    int screenX1, screenY1, screenX2, screenY2;

    bool point1InFront = (z1 > 0);
    bool point2InFront = (z2 > 0);

    if (point1InFront && point2InFront) {
        project(x1, y1, z1, &screenX1, &screenY1);
        project(x2, y2, z2, &screenX2, &screenY2);
        SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
    } else if (point1InFront || point2InFront) {
        float xi, yi, t;
        if (!point1InFront) {
            t = z1 / (z1 - z2);
            xi = x1 + t * (x2 - x1);
            yi = y1 + t * (y2 - y1);
            z1 = 0;
            project(xi, yi, z1 + 0.001f, &screenX1, &screenY1);
            project(x2, y2, z2, &screenX2, &screenY2);
            SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
        } else {
            t = z2 / (z2 - z1);
            xi = x2 + t * (x1 - x2);
            yi = y2 + t * (y1 - y2);
            z2 = 0;
            project(x1, y1, z1, &screenX1, &screenY1);
            project(xi, yi, z2 + 0.001f, &screenX2, &screenY2);
            SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
        }
    }
}

void compute_intersection(float x1, float y1, float z1, float x2, float y2, float z2, float *xi, float *yi) {
    float t = z1 / (z1 - z2);
    *xi = x1 + t * (x2 - x1);
    *yi = y1 + t * (y2 - y1);
}
int main() {

    int counter = 0;

    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream file("./model.obj");
    if (!file.is_open()) {
        return 1;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (line.substr(0, 2) == "f ") {
                face face;
                parse_face(line, face);
                faces.push_back(face);
        }
    }
    file.close();

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    keys = SDL_GetKeyboardState(NULL);

    const unsigned int scaling = 1;
    model.origin.z = 5;
    model.origin.y = 0;
    model.origin.x = 0;
    bool running = true;
    while(running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        //take_input();
        if(moving) {
            vertex start, old;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (const auto& face : faces){ 
                bool first = true;
                for (int i = 0; i < face.vertex_indices.size(); i++ ) {
                    int vi = face.vertex_indices[i] - 1;
                    int x_scale = 0;
                    int y_scale = 0;
                    float x = vertices[vi].x + model.origin.x, y = vertices[vi].y + model.origin.y, z = vertices[vi].z + model.origin.z;
                    project(x, y, z, &x_scale, &y_scale);

                    if(first) {
                        old.x = x;
                        old.y = y;
                        old.z = vertices[vi].z + model.origin.z;
                        start.x = x;
                        start.y = y;
                        start.z = old.z;
   
                        first = false;
                        continue;
                    }
                    else if(i == face.vertex_indices.size()-1) {
                        draw_line(x, y, z, start.x, start.y, start.z, renderer);
                    }
                    draw_line(old.x, old.y, old.z, x, y, z, renderer);
                    
                    old.x = x;
                    old.y = y;
                    old.z = vertices[vi].z + model.origin.z;
                }
            }
            
            counter++;
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - time_start;
            if (elapsed.count() >= 30.0) {
                printf("\nfps:%5.5f\ttime elapsed:%5.5f\tframes computed:%.10i\n", counter/(elapsed.count()), elapsed.count(), counter);
                break;
            }

            SDL_RenderPresent(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            //remove for dynamic input:
            rot_y(DEFAULT_ANGLE);
            moving = true;
            //include for dynamic input:
            //moving = false;
            
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}