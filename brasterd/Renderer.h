#pragma once

#include "Buffer.h"
#include <functional>
#include <glm/glm.hpp>


namespace brasterd {

    class Renderer {
    public:
        Renderer();

        ~Renderer();

        void bind_buffer(Buffer2D<3, unsigned char> &buffer);

        void clear(glm::vec3 color);

        // Renderer methods -
        void point(glm::vec2 pos, glm::vec3 color, int point_size = 1);

        void line(glm::vec2 a, glm::vec2 b, glm::vec3 color);

        void triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 color);

        void shade(std::function<glm::vec3(glm::vec2)> func);

    private:
        bool draw_buffer_check();

        glm::ivec2 discretize(glm::vec2 vec);

        glm::ivec3 discretize(glm::vec3 vec);

        Buffer2D<3, unsigned char> *draw_buffer;
    };

}

