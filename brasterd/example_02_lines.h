#pragma once

#ifdef EXAMPLE_02_LINES_AND_COLOR

#include <iostream>
#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"


int main(int argc, char *argv[]) {
    glm::ivec2 screen_size(1280, 720);
    brasterd::Window window("brasterd is awesome!", screen_size);
    brasterd::Buffer2D<3, unsigned char> screen_buf(screen_size);
    window.set_display_buffer(screen_buf);

    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);

    // Vertex In: x y r g b (5)
    // Vertex Out: x y z w r g b (7)
    // Color Out: r g b a (4)
    brasterd::Shader<5, 7, 4> shader([](brasterd::Attribs<5> in_pos) {
        brasterd::Attribs<7> out(in_pos);

        out.to<glm::vec4>(0) = glm::vec4(in_pos.to<glm::vec3>(0), 1.0f);
        out.to<glm::vec3>(4) = glm::vec3(in_pos.to<glm::vec3>(2));
        return out;
    }, [](brasterd::Attribs<7> in_data) {
        return glm::vec4(in_data.to<glm::vec3>(4), 1.0f);
    });

    brasterd::Buffer1D<5, float> lines({
        // Line coordinate  Color
        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        1.0f, 0.0f,         0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,         0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,         0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,        0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f,        0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,       0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        0.0f, -1.0f,        0.2f, 0.5f, 1.0f,

        0.0f, 0.0f,         1.0f, 1.0f, 1.0f,
        1.0f, -1.0f,        0.2f, 0.5f, 1.0f,
    });

    while (!window.should_close()) {
        window.poll_event();

        renderer.clear(glm::vec3(1.0f, 0.5f, 0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Lines, lines, shader);

        window.update();
    }
    return 0;
}

#endif
