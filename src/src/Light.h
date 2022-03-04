#pragma once

#include <glm/vec3.hpp>

struct Light {
    glm::vec3 colour;
    glm::vec3 position;
    float strength = 1.0f;
};