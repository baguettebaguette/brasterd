#include "Sampler2D.h"


// Two implementations of Sampler2D: float4 & uchar3
template<>
glm::vec4 brasterd::Sampler2D<3, unsigned char>::at_buffer(glm::ivec2 pos) {
    pos.x = glm::clamp(pos.x, 0, buffer.size().x);
    pos.y = glm::clamp(pos.y, 0, buffer.size().y);

    glm::u8vec3 sampled = buffer.at(pos);
    return glm::vec4(sampled.x / 255.0f, sampled.y / 255.0f, sampled.z / 255.0f, 1.0f);
}

template<>
glm::vec4 brasterd::Sampler2D<4, float>::at_buffer(glm::ivec2 pos) {
    pos.x = glm::clamp(pos.x, 0, buffer.size().x);
    pos.y = glm::clamp(pos.y, 0, buffer.size().y);

    return buffer.at(pos);
}
