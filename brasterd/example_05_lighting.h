#pragma once

#ifdef EXAMPLE_05_LIGHTING


#include "Window.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

// Based on example 04. This is just a spinning cube with phong lighting calculated on the fly.


int main(int argc, char* argv[]) {
    brasterd::Window window("brasterd: lighting", glm::ivec2(1280, 720));
    brasterd::Buffer2D<3, unsigned char> canvas(glm::ivec2(1280, 720));
    window.set_display_buffer(canvas);

    brasterd::Buffer1D<6, float> cube({
        // position                 normal
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f
    });

    brasterd::Renderer renderer;
    renderer.bind_buffer(canvas);
    renderer.params.depth_test = true;
    renderer.params.point_size = 5;

    float delta_time = 0.0f;
    float prev_instant = window.clock();

    glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 2.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720, 0.01f, 100.0f);

    // Lighting configurations
    glm::vec3 camera_pos = glm::vec3(0.0f);
    glm::vec3 light_pos = glm::vec3(0.0f, 2.0f, 0.0f);
    glm::vec3 ambient_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse_light_color = glm::vec3(1.0f, 0.5f, 0.0f);
    glm::vec3 specular_light_color = glm::vec3(1.0f);

    // vec4<out_pos 4, modeled_pos 3, normal 3>
    brasterd::Shader<6, 10> shader([&](brasterd::Attribs<6> in_attr) {
        brasterd::Attribs<10> out;
        glm::vec4 modeled_pos = glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        out.to<glm::vec4>(0) = perspective * view * modeled_pos;
        out.to<glm::vec3>(4) = glm::vec3(modeled_pos);
        out.to<glm::vec3>(7) = in_attr.to<glm::vec3>(3);
        return out;
    }, [&](brasterd::Attribs<10> in_attr) {
        glm::vec3 pos = in_attr.to<glm::vec3>(4);
        glm::vec3 normal = glm::vec4(in_attr.to<glm::vec3>(7), 0.0f);
        glm::vec3 light_dir = glm::normalize(light_pos - pos);
        glm::vec3 eye_dir = glm::normalize(camera_pos - pos);
        glm::vec3 reflected = glm::reflect(-light_dir, normal);

        // Lighting calculation!
        float ambient = 0.1f;
        float diffuse = glm::min(1.0f, glm::max(0.0f, glm::dot(light_dir, normal)));
        float specular = glm::pow(glm::min(1.0f, glm::max(0.0f, glm::dot(reflected, eye_dir))), 10.0f) * 0.6f;

        return glm::vec4(ambient * ambient_light_color +
            diffuse * diffuse_light_color + 
            specular * specular_light_color, 1.0f);
    });

    // Render the light -
    brasterd::Buffer1D<3, float> light_buf({
        light_pos.x, light_pos.y, light_pos.z
    });
    brasterd::Shader<3, 4> light_shader([&](brasterd::Attribs<3> in_attr) {
        brasterd::Attribs<4> out;
        out.to<glm::vec4>(0) = perspective * view * glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        return out;
    }, [](brasterd::Attribs<4> in_attr) {
        return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    });

    while (!window.should_close()) {
        window.poll_event();

        camera_pos = glm::vec3(5.0f * sinf(window.clock() * 0.5f), 2.0f + 3.0f * (sinf(window.clock()) * 0.5f + 0.5f), -5.0f * cosf(window.clock() * 0.5f));
        view = glm::lookAt(camera_pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        light_pos = glm::vec3(1.0f * cosf(window.clock()), 1.0f + 0.5f * (sinf(window.clock()) * 0.5f + 0.5f), 1.0f * sinf(window.clock()));
        light_buf.at(0).to<glm::vec3>(0) = light_pos;

        float now = window.clock();
        delta_time = now - prev_instant;
        prev_instant = now;

        renderer.clear(glm::vec3(0.0f, 0.0f, 0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, cube, shader);
        renderer.draw_buffer(brasterd::RenderMode::Points, light_buf, light_shader);

        window.update();
    }

    return 0;
}



#endif

