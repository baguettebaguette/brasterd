#pragma once

#include <glm/glm.hpp>
#include <string>


namespace brasterd {

    template<int ch, typename T>
    class Buffer2D {
    public:
        Buffer2D(glm::ivec2 size) {
            buffer = new T[ch * size.x * size.y];
            buffer_size = size;
            std::memset(buffer, 0, sizeof(T) * ch * size.x * size.y);
        }

        ~Buffer2D() {
            delete[] buffer;
        }

        void resize(glm::ivec2 new_size);

        T& at(int pos) {
            return buffer[pos];
        }

        template<int ch = 3>
        glm::vec<3, T>& at(glm::ivec2 pos) {
            glm::vec<3, T> *ptr = (glm::vec<3, T> *) &(buffer[(pos.y * buffer_size.x + pos.x) * 3]);
            return *ptr;
        }

        glm::ivec2 size() {
            return buffer_size;
        }

        void clear(glm::vec3 color) {
            glm::u8vec3 converted = color * 255.0f;
            for (int y = 0; y < buffer_size.y; y++) {
                for (int x = 0; x < buffer_size.x; x++) {
                    buffer[(y * buffer_size.x + x) * ch + 0] = converted.x;
                    buffer[(y * buffer_size.x + x) * ch + 1] = converted.y;
                    buffer[(y * buffer_size.x + x) * ch + 2] = converted.z;
                }
            }
        }

    private:
        T *buffer;
        glm::ivec2 buffer_size;

        friend class Window;
    };


}
