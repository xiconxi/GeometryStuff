#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

int Triangulate_EC(std::vector<glm::vec2> &poly, std::vector<glm::ivec3>  &triangles);

float Area(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) ;

bool InTriangle(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 p);