#include "Renderer.h"
#include "util.h"

#define DRAW_BUFFER_CHECK() if (!draw_buffer_check()) { return; }
#define BOUNDARY_CHECK(x, y) (x >= 0 && y >= 0 && x < draw_buffer->size().x && y < draw_buffer->size().y)

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

    draw_buffer->clear(discretize(color));
}

void brasterd::Renderer::point(glm::vec2 pos, glm::vec3 color, int point_size) {
    DRAW_BUFFER_CHECK();
    
    glm::ivec2 half = glm::ivec2(point_size) / 2;
    glm::ivec2 d_pos_lb = discretize(pos) - half;
    glm::ivec2 d_pos_ub = discretize(pos) + half;
    for (int y = d_pos_lb.y; y < d_pos_ub.y; y++) {
        for (int x = d_pos_lb.x; x < d_pos_ub.x; x++) {
            if (!BOUNDARY_CHECK(x, y)) {
                continue;
            }
            draw_buffer->at({ x, y }) = discretize(color);
        }
    }
}

void brasterd::Renderer::line(glm::vec2 f_a, glm::vec2 f_b, glm::vec3 f_color) {
    glm::ivec2 a = discretize(f_a);
    glm::ivec2 b = discretize(f_b);
    glm::u8vec3 color = discretize(f_color);

    
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    bool flip_xy = false;
    bool dec_y = false;
    if (glm::abs(dy) > glm::abs(dx)) {
        flip_xy = true;
        swap(a.x, a.y);
        swap(b.x, b.y);
        swap(dx, dy);
    }
    if (a.x > b.x) {
        swap(a, b);
        dx = -dx;
        dy = -dy;
    }
    if (a.y > b.y) {
        dec_y = true;
    }

    int delta = 0;
    int y = a.y;
    for (int x = a.x; x <= b.x; x++) {
        delta += dy;
        if (!dec_y) {
            if (delta >= dx) {
                y += 1;
                delta = delta - dx;
            }
        } else {
            if (delta < dx) {
                y -= 1;
                delta = delta + dx;
            }
        }
        
        if (flip_xy) {
            if (!BOUNDARY_CHECK(y, x)) {
                continue;
            }
            draw_buffer->at(glm::ivec2(y, x)) = color;
        } else {
            if (!BOUNDARY_CHECK(x, y)) {
                continue;
            }
            draw_buffer->at(glm::ivec2(x, y)) = color;
        }
        
    }
}

void brasterd::Renderer::triangle(glm::vec2 f_a, glm::vec2 f_b, glm::vec2 f_c, glm::vec3 f_color) {
    glm::ivec2 a = discretize(f_a);
    glm::ivec2 b = discretize(f_b);
    glm::ivec2 c = discretize(f_c);
    glm::u8vec3 color = discretize(f_color);
    glm::ivec2 bbox_low = glm::ivec2(glm::min(c.x, glm::min(a.x, b.x)), glm::min(c.y, glm::min(a.y, b.y)));
    glm::ivec2 bbox_hi = glm::ivec2(glm::max(c.x, glm::max(a.x, b.x)), glm::max(c.y, glm::max(a.y, b.y)));

    glm::vec3 ab = glm::vec3(f_b - f_a, 0.0f);
    glm::vec3 ac = glm::vec3(f_c - f_a, 0.0f);
    float s = glm::cross(ab, ac).z;
    for (int y = bbox_low.y; y <= bbox_hi.y; y++) {
        for (int x = bbox_low.x; x <= bbox_hi.x; x++) {
            if (!BOUNDARY_CHECK(x, y)) {
                // No point in other calculations anyway
                continue;
            }

            // Calculate barycentric coordinates
            glm::vec2 d = glm::vec2((float) x / draw_buffer->size().x, (float) y / draw_buffer->size().y) * 2.0f - 1.0f;
            glm::vec3 ad = glm::vec3(d - f_a, 0.0f);

            float u = glm::cross(ab, ad).z / s;
            float v = glm::cross(ad, ac).z / s;
            float w = 1.0f - u - v;
            if (w < 0.0f || w > 1.0f) {
                continue;
            }
            draw_buffer->at(glm::ivec2(x, y)) = color;
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
    vec = vec * 0.5f + 0.5f;
    return glm::ivec2(draw_buffer->size().x * vec.x, draw_buffer->size().y * vec.y);
}

glm::u8vec3 brasterd::Renderer::discretize(glm::vec3 vec) {
    return glm::u8vec3(255.0f * vec.x, 255.0f * vec.y, 255.0f * vec.z);
}
