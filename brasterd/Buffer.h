#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>


namespace brasterd {

    template<int ch, typename T>
    struct Buffer2D {
        Buffer2D() {
            buffer_size = glm::ivec2(0);
            buffer = nullptr;
        }

        Buffer2D(glm::ivec2 size) {
            buffer = new T[ch * size.x * size.y];
            buffer_size = size;
            std::memset(buffer, 0, sizeof(T) * ch * size.x * size.y);
        }

        Buffer2D(Buffer2D &&that) {
            // Move pointer to prevent double-free
            // Maybe use std::shared_ptr to let them manage instead?
            // Nope, fuck that
            this->buffer = that.buffer;
            this->buffer_size = that.buffer_size;
            that.buffer = nullptr;
        }

        ~Buffer2D() {
            if (buffer) {
                delete[] buffer;
            }
        }

        void resize(glm::ivec2 new_size) {
            if (!buffer) {
                buffer = new T[ch * new_size.x * new_size.y];
                std::memset(buffer, 0, sizeof(T) * ch * new_size.x * new_size.y);
                buffer_size = new_size;
                return;
            }

            T *new_buffer = new T[ch * new_size.x * new_size.y];
            // Naive resampling: just turn them into float & turn them BACK into ints. Aliasing LETS GO
            for (int y = 0; y < new_size.y; y++) {
                for (int x = 0; x < new_size.x; x++) {
                    glm::vec2 sample_pos((float) y / new_size.y, x / new_size.x);
                    glm::ivec2 sample_pos_discrete = glm::ivec2(sample_pos.x * size().x, sample_pos.y * size().y);
                    
                    for (int i = 0; i < ch; i++) {
                        new_buffer[(y * new_size.x + x) * ch + i] = buffer[(sample_pos_discrete.y * size().x + x) * ch + i];
                    }
                    
                }
            }
            delete[] buffer;
            buffer = new_buffer;
            buffer_size = new_size;
        }

        T& operator[](int pos) {
            return buffer[pos];
        }

        glm::vec<ch, T> &at(glm::ivec2 pos) {
            glm::vec<ch, T> *ptr = (glm::vec<ch, T> *) &(buffer[(pos.y * buffer_size.x + pos.x) * ch]);
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

        void buffer_data(const void *ptr, int offset, int size);

        T *buffer;
        glm::ivec2 buffer_size;
    };


    template<int ch>
    struct Attribs {
        Attribs() : data() {}

        template<int ch2>
        Attribs<ch>(Attribs<ch2> another) {
            int min = glm::min(ch, ch2);
            for (int i = 0; i < min; i++) {
                data[i] = another.data[i];
            }
            for (int i = min; i < ch; i++) {
                data[i] = 0.0f;
            }
        }

        template<int ch2>
        Attribs<ch>(glm::vec<ch2, float> another) {
            int min = glm::min(ch, ch2);
            for (int i = 0; i < min; i++) {
                data[i] = another[i];
            }
            for (int i = min; i < ch; i++) {
                data[i] = 0.0f;
            }
        }

        ~Attribs() {}

        template<typename T>
        T &to(int offset) {
            T *ptr = (T *) &data[offset];
            return *ptr;
        }

        float data[ch];
    };

    template<int ch>
    Attribs<ch> operator*(float constant, Attribs<ch> attr) {
        Attribs<ch> ret_attr;
        for (int i = 0; i < ch; i++) {
            ret_attr.data[i] = constant * attr.data[i];
        }
        return ret_attr;
    }

    template<int ch>
    Attribs<ch> operator+(Attribs<ch> a, Attribs<ch> b) {
        Attribs<ch> ret_attr;
        for (int i = 0; i < ch; i++) {
            ret_attr.data[i] = a.data[i] + b.data[i];
        }
        return ret_attr;
    }


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

        Buffer1D(Buffer1D&& that) {
            buffer = that.buffer;
            buffer_size = that.buffer_size;
            that.buffer = nullptr;
        }

        ~Buffer1D() {
            if (buffer) {
                delete[] buffer;
            }
        }

        void resize();

        T &operator[](int pos) {
            return buffer[pos];
        }

        Attribs<ch> &at(int pos) {
            Attribs<ch> *ptr = (Attribs<ch> *) &buffer[pos * ch];
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

        void buffer_data(const void *ptr, int offset, int size) {
            std::memcpy(&buffer[offset], ptr, size);
        }

        T *buffer;
        int buffer_size;
    };

}
