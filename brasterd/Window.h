#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Buffer.h"


namespace brasterd {

    class Window {
    public:
        Window(std::string title, glm::ivec2 resolution);

        void set_display_buffer(Buffer2D<3, unsigned char> &buffer);

        ~Window();

        bool should_close();

        void update();

        void poll_events();

        glm::ivec2 size();

        void resize(glm::ivec2 size);

        void set_title(std::string new_title);

        float clock();

    private:
        GLuint compile(GLuint shader_type, const char *src);

        GLFWwindow *window;
        Buffer2D<3, unsigned char> *display_buffer;

        GLuint rect_vao, rect_vbo;
        GLuint program, tex_location;
        GLuint texture;
        glm::ivec2 window_size;
    };
}


