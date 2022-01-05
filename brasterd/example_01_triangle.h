#pragma once


#ifdef EXAMPLE_01_TRIANGLE

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

    brasterd::Shader<3, 4, 4> shader([](brasterd::Attribs<3> in_pos) {
        return brasterd::Attribs<4>(in_pos);
    }, [](brasterd::Attribs<4> in_data) {
        return glm::vec4(0.2f, 0.5f, 1.0f, 1.0f);
    });

    brasterd::Buffer1D<3, float> triangle({
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    });

    while (!window.should_close()) {
        window.poll_event();

        renderer.clear(glm::vec3(1.0f, 0.5f, 0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, triangle, shader);

        window.update();
    }
    return 0;
}


#endif
