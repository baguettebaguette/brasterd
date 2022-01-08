#include "Sampler2D.h"

brasterd::Sampler2D::Sampler2D(Buffer2D<4, float>& buffer) : buffer(buffer) {
  
}

brasterd::Sampler2D::~Sampler2D() {
}

glm::vec4& brasterd::Sampler2D::at(glm::vec2 pos) {
    if (pos.x < 0.0f || pos.x > 1.0f) {
        switch (params.wrap_s) {
        case Repeat:
            pos.x = pos.x - glm::floor(pos.x);
            break;

        case ClampToEdge:
            if (pos.x < 0.0f) {
                pos.x = 0.0f;
            } else {
                pos.x = 1.0f;
            }
            break;

        case ClampToBorder:
            return params.border_color;
        }
    }
    if (pos.y < 0.0f || pos.y > 1.0f) {
        switch (params.wrap_t) {
        case Repeat:
            pos.y = pos.y - glm::floor(pos.y);
            break;

        case ClampToEdge:
            if (pos.y < 0.0f) {
                pos.y = 0.0f;
            } else {
                pos.y = 1.0f;
            }
            break;

        case ClampToBorder:
            return params.border_color;
        }
    }
    glm::vec2 unnormalized = glm::ivec2(pos.x * buffer.size().x, pos.y * buffer.size().y);
    glm::vec4 ret(1.0f);
    switch (params.filter) {
    case Linear:
        // Bilinear filtering
        glm::ivec2 u = glm::floor(unnormalized);
        glm::vec4 a = at_buffer(u);
        glm::vec4 b = at_buffer(u + glm::ivec2(1, 0));
        glm::vec4 c = at_buffer(u + glm::ivec2(0, 1));
        glm::vec4 d = at_buffer(u + glm::ivec2(1, 1));
        glm::vec2 f = glm::fract(unnormalized);

        ret = glm::mix(glm::mix(a, b, f.x), glm::mix(c, d, f.x), f.y);
        break;

    case Nearest:
        glm::ivec2 nearest = glm::round(unnormalized);
        ret = at_buffer(nearest);
        break;
    }
    return ret;
}

glm::vec4 brasterd::Sampler2D::at_buffer(glm::ivec2 pos) {
    pos.x = glm::clamp(pos.x, 0, buffer.size().x);
    pos.y = glm::clamp(pos.y, 0, buffer.size().y);

    return buffer.at(pos);
}
