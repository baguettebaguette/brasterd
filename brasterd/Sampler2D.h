#pragma once

#include "Buffer.h"


namespace brasterd {

    enum SamplerFilterParams {
        Nearest,
        Linear
    };

    enum SamplerWrapParams {
        Repeat,
        // MirrorRepeat, Not doing this right now
        ClampToEdge,
        ClampToBorder
    };

    struct SamplerParams {
        SamplerFilterParams filter = Nearest;
        SamplerWrapParams wrap_s = Repeat;
        SamplerWrapParams wrap_t = Repeat;
        glm::vec4 border_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };


    template<int ch, typename T>
    class Sampler2D {
    public:
        Sampler2D(Buffer2D<ch, T>& buffer) : buffer(buffer) {
        }

        ~Sampler2D() {
        }

        glm::vec4 at(glm::vec2 pos) {
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
            glm::vec2 unnormalized = glm::vec2(pos.x * buffer.size().x, pos.y * buffer.size().y);
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

                return glm::mix(glm::mix(a, b, f.x), glm::mix(c, d, f.x), f.y);
                break;

            case Nearest:
                glm::ivec2 nearest = glm::round(unnormalized);
                return at_buffer(nearest);
            }
            return params.border_color;
        }

        SamplerParams params;

    private:
        glm::vec4 at_buffer(glm::ivec2 pos);

        Buffer2D<ch, T> &buffer;
    };
}

