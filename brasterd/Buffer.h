#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>


namespace brasterd {

    template<int ch, typename T>
    struct Buffer2D {
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

        T *buffer;
        glm::ivec2 buffer_size;
    };


    template<int ch, typename T>
    struct Buffer1D {
        Buffer1D(int len) {
            buffer = new T[ch * len];
            buffer_size = len;
        }

        Buffer1D(std::initializer_list<T> initial_data) {
            std::vector<T> data;
            for (T t : initial_data) {
                data.push_back(t);
            }
            buffer = new T[ch * data.size()];
            std::memcpy(buffer, &data[0], sizeof(T) * data.size());
            buffer_size = (int) data.size() / ch;
        }

        ~Buffer1D() {
            delete[] buffer;
        }

        void resize();

        T &operator[](int pos) {
            return buffer[pos];
        }

        glm::vec<ch, T> &at(int pos) {
            glm::vec<ch, T> *ptr = (glm::vec<ch, T> *) &buffer[pos * ch];
            return *ptr;
        }

        int size() {
            return buffer_size;
        }

        void clear(T what) {
            for (int i = 0; i < buffer_size; i++) {
                buffer[i] = what;
            }
        }

        T *buffer;
        int buffer_size;
    };

}
