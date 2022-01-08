#include "util.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace brasterd {

    Buffer2D<4, float> brasterd::read_image(std::string path) {
        stbi_set_flip_vertically_on_load(true);
        glm::ivec2 size(0, 0);
        int channels = 0;
        unsigned char *data = stbi_load(path.c_str(), &size.x, &size.y, &channels, 0);
        if (!data) {
            throw "Unable to find image: " + path;
        }

        Buffer2D<4, float> ret(size);
        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                glm::vec4 color(1.0f);
                for (int ch = 0; ch < channels; ch++) {
                    color[ch] = data[(y * size.x + x) * channels + ch] / 255.0f;
                }
                ret.at(glm::ivec2(x, y)) = color;
            }
        }
        stbi_image_free(data);

        return ret;
    }


}

