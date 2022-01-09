#pragma once


#ifdef EXAMPLE_08_FRAMEBUFFER

#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Buffer.h"
#include "Shader.h"
#include "Renderer.h"

// Framebuffer is an emergent(?) property of Renderer & Buffer2D. So I don't think we need
// anything new.
int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: framebuffer", glm::ivec2(1280, 720));

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
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f);

    brasterd::Shader<3, 4> shader([&](brasterd::Attribs<3> in_attr) {
        brasterd::Attribs<4> out;
        out.to<glm::vec4>(0) = perspective * view * glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        return out;
    }, [](brasterd::Attribs<4> in_attr) {
        return glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    });

    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1280, 720));
    window.set_display_buffer(screen_buf);
    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);
    renderer.params.depth_test = true;

    while (!window.should_close()) {
        window.poll_events();
        view = glm::lookAt(glm::vec3(5.0f * sinf(window.clock()), 0.1f, 5.0f * cosf(window.clock())), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.clear(glm::vec3(0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, cube, shader);
        window.update();
    }
}

#endif

