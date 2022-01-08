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

    class Sampler2D {
    public:
        Sampler2D(Buffer2D<4, float> &buffer);

        ~Sampler2D();

        glm::vec4 &at(glm::vec2 pos);

        SamplerParams params;

    private:
        glm::vec4 at_buffer(glm::ivec2 pos);

        Buffer2D<4, float> &buffer;
    };

}

