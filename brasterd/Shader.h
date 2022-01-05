#pragma once

#include <functional>
#include <glm/glm.hpp>


template<typename T>
class Shader {
public:
    Shader(std::function<glm::vec4(T&)> vertex_shader,
        std::function<glm::vec4(T&)> fragment_shader) {

    }

    ~Shader() {

    }
};

