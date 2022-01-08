#pragma once


#ifdef EXAMPLE_07_MODEL_TEXTURE

#include <glm/gtc/matrix_transform.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Sampler2D.h"
#include "util.h"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

int main(int argc, char* argv) {
    brasterd::Window window("brasterd: textured model", glm::ivec2(1280, 720));

    brasterd::Buffer2D<4, float> image = brasterd::read_image("models/suzanne/suzanne.png");
    brasterd::Sampler2D suzanne_texture(image);

    // Read model using tinyobjloader. I am not implementing an obj reader.
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig tinyobj_reader_config;
    tinyobj_reader_config.mtl_search_path = "./models/suzanne/";
    if (!reader.ParseFromFile("models/suzanne/suzanne.obj", tinyobj_reader_config)) {
        throw "Could not load suzanne.obj.";
    }
    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();
    std::vector<Vertex> vertices;
    
    for (int s = 0; s < shapes.size(); s++) {
        for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            for (int v = 0; v < 3; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[3 * f + v];
                vertices.push_back({
                    glm::vec3(attrib.vertices[3 * idx.vertex_index + 0], 
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]),

                    glm::vec3(attrib.vertices[3 * idx.normal_index + 0], 
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]),

                    glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0], 
                        attrib.texcoords[2 * idx.texcoord_index + 1])
                });
            }
        }
    }
    brasterd::Buffer1D<8, float> data((int) vertices.size() * 8);
    data.buffer_data(&vertices[0], 0, sizeof(Vertex) * (int) vertices.size());

    brasterd::Buffer2D<3, unsigned char> screen_buf(glm::ivec2(1280, 720));
    window.set_display_buffer(screen_buf);
    brasterd::Renderer renderer;
    renderer.bind_buffer(screen_buf);
    renderer.params.depth_test = true;

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f);

    brasterd::Shader<8, 9> shader([&](brasterd::Attribs<8> in_attr) {
        brasterd::Attribs<9> out;
        out.to<glm::vec4>(0) = perspective * view * glm::vec4(in_attr.to<glm::vec3>(0), 1.0f);
        out.to<glm::vec3>(4) = in_attr.to<glm::vec3>(3);
        out.to<glm::vec2>(7) = in_attr.to<glm::vec2>(6);
        return out;
    }, [&](brasterd::Attribs<9> in_attr) {
        return suzanne_texture.at(in_attr.to<glm::vec2>(7));
    });

    while (!window.should_close()) {
        window.poll_event();

        view = glm::lookAt(glm::vec3(4.0f * sinf(window.clock()), 0.0f, 4.0f * cosf(window.clock())), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        renderer.clear(glm::vec3(0.0f, 0.0f, 0.0f));
        renderer.draw_buffer(brasterd::RenderMode::Triangles, data, shader);

        window.update();
    }
    return 0;
}

#endif
