#pragma once


#ifdef EXAMPLE_03_DEPTH_TEST

#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"


int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: Depth Test", glm::ivec2(1280, 720));

    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1280, 720));
    window.set_display_buffer(screen_buf);

    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);

    while (!window.should_close()) {
        window.poll_event();

        renderer.clear(glm::vec3(1.0f, 0.5f, 0.0f));

        window.update();
    }

    return 0;
}

#endif
