#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "parsing.hpp"
#include <hirie.hpp>

void hirie_parse_face(const std::string& line, hirie_face& face) {
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

int hirie_parse_obj(std::vector<hirie_vertex>& vertices, std::vector<hirie_face>& faces, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return 1;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;
        if (prefix == "v") {
            hirie_vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (line.substr(0, 2) == "f ") {
                hirie_face face;
                hirie_parse_face(line, face);
                faces.push_back(face);
        }
    }
    file.close();
    return 0;
}