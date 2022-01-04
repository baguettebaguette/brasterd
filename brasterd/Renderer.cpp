#include "Renderer.h"

#define DRAW_BUFFER_CHECK() if (!draw_buffer_check()) { return; }
#define BOUNDARY_CHECK(a) if (a.x < 0.0f || a.y < 0.0f || a.x > 1.0f || a.y > 1.0f) { return; }

brasterd::Renderer::Renderer() {
    draw_buffer = nullptr;
}

brasterd::Renderer::~Renderer() {
}

void brasterd::Renderer::bind_buffer(Buffer2D<3, unsigned char>& buffer) {
    draw_buffer = &buffer;
}

void brasterd::Renderer::clear(glm::vec3 color) {
    DRAW_BUFFER_CHECK();
    draw_buffer->clear(color);
}

void brasterd::Renderer::point(glm::vec2 pos, glm::vec3 color, int point_size) {
    DRAW_BUFFER_CHECK();
    BOUNDARY_CHECK(pos);
    
    glm::ivec2 half = glm::ivec2(point_size) / 2;
    glm::ivec2 d_pos_lb = discretize(pos) - half;
    glm::ivec2 d_pos_ub = discretize(pos) + half;
    for (int y = d_pos_lb.y; y < d_pos_ub.y; y++) {
        for (int x = d_pos_lb.x; x < d_pos_ub.x; x++) {
            draw_buffer->at({ x, y }) = discretize(color);
        }
    }
}

void brasterd::Renderer::shade(std::function<glm::vec3(glm::vec2)> func) {
    DRAW_BUFFER_CHECK();

    for (int y = 0; y < draw_buffer->size().y; y++) {
        for (int x = 0; x < draw_buffer->size().x; x++) {
            glm::vec2 v_in = glm::vec2((float) x / draw_buffer->size().x, (float) y / draw_buffer->size().y);
            glm::ivec3 out = discretize(func(v_in));
            draw_buffer->at((y * draw_buffer->size().x + x) * 3 + 0) = (unsigned char) out.r;
            draw_buffer->at((y * draw_buffer->size().x + x) * 3 + 1) = (unsigned char) out.g;
            draw_buffer->at((y * draw_buffer->size().x + x) * 3 + 2) = (unsigned char) out.b;
        }
    }
}

bool brasterd::Renderer::draw_buffer_check() {
    return draw_buffer;
}

glm::ivec2 brasterd::Renderer::discretize(glm::vec2 vec) {
    return glm::ivec2(draw_buffer->size().x * vec.x, draw_buffer->size().y * vec.y);
}

glm::ivec3 brasterd::Renderer::discretize(glm::vec3 vec) {
    return glm::ivec3(255.0f * vec.x, 255.0f * vec.y, 255.0f * vec.z);
}
