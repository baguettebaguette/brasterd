#pragma once


#ifdef EXAMPLE_03_DEPTH_TEST

#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"


int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: depth test", glm::ivec2(1280, 720));

    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1280, 720));
    window.set_display_buffer(screen_buf);

    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);
    renderer.params.depth_test = true;

    brasterd::Buffer1D<6, float> triangles = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.25f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.15f, 0.15f, 0.1f, 0.0f, 0.0f, 1.0f,
        0.65f, 0.15f, 0.1f, 0.0f, 0.0f, 1.0f,
        0.4f, 0.65f, 0.1f, 0.0f, 0.0f, 1.0f,
    };

    brasterd::Shader<6, 7> shader([](brasterd::Attribs<6> attr_in) {
        brasterd::Attribs<7> ret;

        ret.to<glm::vec4>(0) = glm::vec4(attr_in.to<glm::vec3>(0), 1.0f);
        ret.to<glm::vec3>(4) = attr_in.to<glm::vec3>(3);
        return ret;
    }, [](brasterd::Attribs<7> attr_in) {
        return glm::vec4(attr_in.to<glm::vec3>(4), 1.0f);
    });

    while (!window.should_close()) {
        window.poll_events();

        renderer.clear(glm::vec3(1.0f, 1.0f, 1.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, triangles, shader);

        window.update();
    }

    return 0;
}

#endif
