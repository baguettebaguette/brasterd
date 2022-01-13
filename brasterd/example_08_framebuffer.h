#pragma once


#ifdef EXAMPLE_08_FRAMEBUFFER

#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Buffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Sampler2D.h"
#include "util.h"


// Framebuffer is an emergent(?) property of Renderer & Buffer2D. So I don't think we need
// anything new.
int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: framebuffer", glm::ivec2(1280, 720));

    // Things in the framebuffer
    brasterd::Buffer2D<4, float> image = brasterd::read_image("img/avatar.png");
    brasterd::Sampler2D<4, float> texture(image);
    brasterd::Buffer1D<2, float> rect({
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f
    });
    brasterd::Shader<2, 4> fb_shader([](brasterd::Attribs<2> in_attr) {
        brasterd::Attribs<4> out;
        out.to<glm::vec4>(0) = glm::vec4(in_attr.to<glm::vec2>(0), 0.0f, 1.0f);
        return out;
    }, [&](brasterd::Attribs<4> in_attr) {
        glm::vec2 uv = in_attr.to<glm::vec2>(0) * 0.5f + 0.5f;
        glm::vec3 inverted = 1.0f - glm::vec3(texture.at(uv));
        return glm::vec4(inverted, 1.0f);
    });
    brasterd::Buffer2D<3, unsigned char> framebuffer(glm::ivec2(100, 100));
    brasterd::Renderer fb_renderer;
    fb_renderer.bind_buffer(framebuffer);

    brasterd::Sampler2D<3, unsigned char> sampler(framebuffer);
    brasterd::Buffer1D<4, float> cube({
        -0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f,

        -0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f
    });
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f);

    brasterd::Shader<4, 7> shader([&](brasterd::Attribs<4> in_attr) {
        brasterd::Attribs<7> out;
        out.to<glm::vec4>(0) = perspective * view * glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        out.to<float>(4) = in_attr.to<float>(3);
        out.to<glm::vec2>(5) = (in_attr.to<glm::vec2>(0) * 2.0f) * 0.5f + 0.5f;
        return out;
    }, [&](brasterd::Attribs<7> in_attr) {
        float should_sample = in_attr.to<float>(4);
        if (should_sample == 0.0f) {
            return glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        } else {
            glm::vec2 uv = in_attr.to<glm::vec2>(5);
            return sampler.at(uv);
        }
    });

    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1280, 720));
    window.set_display_buffer(screen_buf);
    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);
    renderer.params.depth_test = true;
    window.set_display_buffer(screen_buf);

    while (!window.should_close()) {
        window.poll_events();
        view = glm::lookAt(glm::vec3(5.0f * sinf(window.clock()), 0.1f, 5.0f * cosf(window.clock())), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        fb_renderer.clear(glm::vec3(0.0f));
        fb_renderer.draw_buffer(brasterd::RenderMode::Triangles, rect, fb_shader);

        renderer.clear(glm::vec3(0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, cube, shader);

        
        window.update();
    }
}

#endif

