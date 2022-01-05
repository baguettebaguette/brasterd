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

        void clear(glm::vec<ch, T> what) {
            for (int y = 0; y < buffer_size.y; y++) {
                for (int x = 0; x < buffer_size.x; x++) {
                    for (int c = 0; c < ch; c++) {
                        buffer[(y * buffer_size.x + x) * ch + c] = what[c];
                    }
                }
            }
        }

    private:
        T *buffer;
        glm::ivec2 buffer_size;

        friend class Window;
    };


}
