#include "Renderer.h"
#include "util.h"


brasterd::Renderer::Renderer() {
    target_buffer = nullptr;
}

brasterd::Renderer::~Renderer() {
}

void brasterd::Renderer::bind_buffer(Buffer2D<3, unsigned char>& buffer) {
    target_buffer = &buffer;
    depth_buffer.resize(buffer.size());
}

void brasterd::Renderer::clear(glm::vec3 color) {
    BRASTERD_TARGET_BUFFER_CHECK();

    target_buffer->clear(discretize(color));
}

bool brasterd::Renderer::target_buffer_check() {
    return target_buffer;
}

glm::ivec2 brasterd::Renderer::discretize(glm::vec2 vec) {
    vec = vec * 0.5f + 0.5f;
    return glm::ivec2(target_buffer->size().x * vec.x, target_buffer->size().y * vec.y);
}

glm::u8vec3 brasterd::Renderer::discretize(glm::vec3 vec) {
    return glm::u8vec3(255.0f * vec.x, 255.0f * vec.y, 255.0f * vec.z);
}
