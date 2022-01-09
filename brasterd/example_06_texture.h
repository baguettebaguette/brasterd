#pragma once


#ifdef EXAMPLE_06_TEXTURE

#include "Window.h"
#include "Buffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Sampler2D.h"
#include "util.h"


int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: texture", glm::ivec2(1000, 1000));
    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1000, 1000));
    window.set_display_buffer(screen_buf);

    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);

    brasterd::Buffer1D<3, float> rect = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };

    brasterd::Buffer2D<4, float> image = brasterd::read_image("img/avatar.png");
    brasterd::Sampler2D texture(image);
    texture.params.filter = brasterd::Linear;

    brasterd::Shader<3, 4> shader([](brasterd::Attribs<3> in_attr) {
        brasterd::Attribs<4> out;
        out.to<glm::vec4>(0) = glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        return out;
    }, [&](brasterd::Attribs<4> in_attr) {
        glm::vec2 uv = in_attr.to<glm::vec2>(0) * 0.5f + 0.5f;
        glm::vec4 sampled = texture.at(uv);
        return sampled;
    });

    renderer.clear(glm::vec3(0.0f, 0.0f, 0.0f));
    renderer.draw_buffer(brasterd::RenderMode::Triangles, rect, shader);

    while (!window.should_close()) {
        window.poll_events();

        window.update();
    }
    return 0;
}

#endif
