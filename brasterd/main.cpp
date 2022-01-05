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

    brasterd::Shader<3, 4, 4> shader([](glm::vec3 in_pos) {
        return glm::vec4(in_pos, 1.0f);
    }, [](glm::vec4 in_data) {
        return glm::vec4(in_data.x, in_data.y, 1.0f, 1.0f);
    });

    brasterd::Shader<2, 4, 4> shader_lines([](glm::vec2 in_pos) {
        return glm::vec4(in_pos, 0.0f, 1.0f);
    }, [](glm::vec4 in_data) {
        return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    });

    brasterd::Buffer1D<3, float> triangle({
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    });

    brasterd::Buffer1D<2, float> lines({
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        -1.0f, -1.0f,
        0.0f, 0.0f,
        -1.0f, 0.0f,
        0.0f, 0.0f,
        -1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, -1.0f,
        0.0f, 0.0f,
        1.0f, -1.0f
    });

    while (!window.should_close()) {
        window.poll_event();

        renderer.clear(glm::vec3(1.0f, 0.5f, 0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Lines, lines, shader_lines);
        renderer.draw_buffer(brasterd::RenderMode::Triangles, triangle, shader);

        window.update();
    }
    return 0;
}
