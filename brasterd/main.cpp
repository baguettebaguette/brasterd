#include <iostream>
#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"


int main(int argc, char *argv[]) {
    glm::ivec2 screen_size(1280, 720);
    brasterd::Window window("brasterd is awesome!", screen_size);
    brasterd::Buffer2D<3, unsigned char> screen_buf(screen_size);
    window.set_display_buffer(screen_buf);

    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);

    while (!window.should_close()) {
        window.poll_event();

        renderer.clear(glm::vec3(1.0f, 0.5f, 0.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(1.0f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f)); 
        renderer.line(glm::vec2(0.0), glm::vec2(0.7f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(-0.7f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(-1.0f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(-1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(-0.7f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(0.7f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.line(glm::vec2(0.0), glm::vec2(1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f));

        window.update();
    }
    return 0;
}
