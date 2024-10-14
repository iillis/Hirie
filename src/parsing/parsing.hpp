#pragma once

#include <hirie.hpp>
#include <vector>
#include <string>

int hirie_parse_obj(std::vector<hirie_vertex>& vertices, std::vector<hirie_face>& faces, const std::string& filepath);