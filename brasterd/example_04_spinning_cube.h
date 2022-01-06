#pragma once


#ifdef EXAMPLE_04_SPINNING_CUBE

#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>


int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: spinning cube", glm::ivec2(1280, 720));
    brasterd::Buffer2D<3, unsigned char> canvas(glm::ivec2(1280, 720));
    window.set_display_buffer(canvas);

    brasterd::Buffer1D<3, float> cube({
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,

        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
    });

    brasterd::Renderer renderer;
    renderer.bind_buffer(canvas);

    float delta_time = 0.0f;
    float prev_instant = window.clock();

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720, 0.01f, 100.0f);

    brasterd::Shader<3, 4> shader([&](brasterd::Attribs<3> in_attr) {
        brasterd::Attribs<4> out;
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), prev_instant, glm::vec3(0.0f, 1.0f, 0.0f));
        out.to<glm::vec4>(0) = perspective * view * rot * glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        return out;
    }, [](brasterd::Attribs<4> in_attr) {
        return glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
    });

    while (!window.should_close()) {
        window.poll_event();

        float now = window.clock();
        delta_time = now - prev_instant;
        prev_instant = now;

        renderer.clear(glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, cube, shader);

        window.update();
    }

    return 0;
}

#endif
