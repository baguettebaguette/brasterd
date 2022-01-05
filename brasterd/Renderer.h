#pragma once

#include "Buffer.h"
#include <functional>
#include <glm/glm.hpp>
#include "Shader.h"
#include "util.h"

#define BRASTERD_TARGET_BUFFER_CHECK() if (!target_buffer_check()) { return; }
#define BRASTERD_BOUNDARY_CHECK(x, y) (x >= 0 && y >= 0 && x < target_buffer->size().x && y < target_buffer->size().y)


namespace brasterd {

    enum class RenderMode {
        Points, Lines, Triangles
    };

    class Renderer {
    public:
        Renderer();

        ~Renderer();

        void bind_buffer(Buffer2D<3, unsigned char> &buffer);

        void clear(glm::vec3 color);

        // Renderer methods -
        template<int v_ch_in, int v_ch_out, int f_ch_out>
        void point(glm::vec<v_ch_out, float> &vertex_data, Shader<v_ch_in, v_ch_out, f_ch_out> &shader, int point_size = 1) {
            BRASTERD_TARGET_BUFFER_CHECK();

            glm::vec2 pos(vertex_data);
            glm::ivec2 half = glm::ivec2(point_size) / 2;
            glm::ivec2 d_pos_lb = discretize(pos) - half;
            glm::ivec2 d_pos_ub = discretize(pos) + half;
            for (int y = d_pos_lb.y; y < d_pos_ub.y; y++) {
                for (int x = d_pos_lb.x; x < d_pos_ub.x; x++) {
                    if (!BRASTERD_BOUNDARY_CHECK(x, y)) {
                        continue;
                    }
                    glm::vec<f_ch_out, float> color = shader.fragment_shader(vertex_data);
                    target_buffer->at({ x, y }) = discretize(glm::vec3(color));
                }
            }
        }

        template<int v_ch_in, int v_ch_out, int f_ch_out>
        void line(glm::vec<v_ch_out, float> f_a, glm::vec<v_ch_out, float> f_b, Shader<v_ch_in, v_ch_out, f_ch_out> &shader) {
            BRASTERD_TARGET_BUFFER_CHECK();

            glm::ivec2 a = discretize(glm::vec2(f_a));
            glm::ivec2 b = discretize(glm::vec2(f_b));

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

                float progress = ((float) (x - a.x)) / dx;
                glm::vec<v_ch_out, float> interpolated = glm::mix(f_a, f_b, progress);
                glm::u8vec3 color = discretize(glm::vec3(shader.fragment_shader(interpolated)));

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
                    if (!BRASTERD_BOUNDARY_CHECK(y, x)) {
                        continue;
                    }
                    target_buffer->at(glm::ivec2(y, x)) = color;
                } else {
                    if (!BRASTERD_BOUNDARY_CHECK(x, y)) {
                        continue;
                    }
                    target_buffer->at(glm::ivec2(x, y)) = color;
                }

            }
        }


        template<int v_ch_in, int v_ch_out, int f_ch_out>
        void triangle(glm::vec<v_ch_out, float> in_a, glm::vec<v_ch_out, float> in_b, glm::vec<v_ch_out, float> in_c, Shader<v_ch_in, v_ch_out, f_ch_out> &shader) {
            BRASTERD_TARGET_BUFFER_CHECK();

            glm::vec2 f_a = glm::vec2(in_a);
            glm::vec2 f_b = glm::vec2(in_b);
            glm::vec2 f_c = glm::vec2(in_c);

            glm::ivec2 a = discretize(f_a);
            glm::ivec2 b = discretize(f_b);
            glm::ivec2 c = discretize(f_c);
            glm::ivec2 bbox_low = glm::ivec2(glm::min(c.x, glm::min(a.x, b.x)), glm::min(c.y, glm::min(a.y, b.y)));
            glm::ivec2 bbox_hi = glm::ivec2(glm::max(c.x, glm::max(a.x, b.x)), glm::max(c.y, glm::max(a.y, b.y)));

            glm::vec3 ab = glm::vec3(f_b - f_a, 0.0f);
            glm::vec3 ac = glm::vec3(f_c - f_a, 0.0f);
            float s = glm::cross(ab, ac).z;
            for (int y = bbox_low.y; y <= bbox_hi.y; y++) {
                for (int x = bbox_low.x; x <= bbox_hi.x; x++) {
                    if (!BRASTERD_BOUNDARY_CHECK(x, y)) {
                        // No point in other calculations anyway
                        continue;
                    }

                    // Calculate barycentric coordinates
                    glm::vec2 d = glm::vec2((float) x / target_buffer->size().x, (float) y / target_buffer->size().y) * 2.0f - 1.0f;
                    glm::vec3 ad = glm::vec3(d - f_a, 0.0f);

                    float u = glm::cross(ab, ad).z / s;
                    float v = glm::cross(ad, ac).z / s;
                    float w = 1.0f - u - v;
                    if (w < 0.0f || w > 1.0f) {
                        continue;
                    }

                    glm::vec<v_ch_out, float> interpolated = u * in_c + v * in_b + w * in_a;
                    glm::vec<f_ch_out, float> color = shader.fragment_shader(interpolated);
                    target_buffer->at(glm::ivec2(x, y)) = discretize(glm::vec3(color));
                }
            }
        }


        template<int ch, int v_ch_out, int f_ch_out>
        void draw_buffer(RenderMode mode, Buffer1D<ch, float> &buffer, Shader<ch, v_ch_out, f_ch_out> &shader) {
            if (v_ch_out < 4 || f_ch_out < 4) {
                throw "draw_buffer won't work with a vertex or fragment output vector with less than 4 channels";
            }
            switch (mode) {
            case RenderMode::Points:
                for (int i = 0; i < buffer.size(); i++) {
                    glm::vec<ch, float> v_in = buffer.at(i);
                    glm::vec<v_ch_out, float> v_out = shader.vertex_shader(v_in);
                    // TODO: Perspective divide
                    point(v_out, shader, 1);
                }
                break;

            case RenderMode::Lines:
                for (int i = 0; i < buffer.size(); i += 2) {
                    glm::vec<ch, float> v_in_0 = buffer.at(i);
                    glm::vec<ch, float> v_in_1 = buffer.at(i + 1);
                    glm::vec<v_ch_out, float> v_out_0 = shader.vertex_shader(v_in_0);
                    glm::vec<v_ch_out, float> v_out_1 = shader.vertex_shader(v_in_1);

                    // TODO: Perspective divide
                    line(v_out_0, v_out_1, shader);
                }
                break;


            case RenderMode::Triangles:

                for (int i = 0; i < buffer.size(); i += 23) {
                    glm::vec<ch, float> v_in_0 = buffer.at(i);
                    glm::vec<ch, float> v_in_1 = buffer.at(i + 1);
                    glm::vec<ch, float> v_in_2 = buffer.at(i + 2);
                    glm::vec<v_ch_out, float> v_out_0 = shader.vertex_shader(v_in_0);
                    glm::vec<v_ch_out, float> v_out_1 = shader.vertex_shader(v_in_1);
                    glm::vec<v_ch_out, float> v_out_2 = shader.vertex_shader(v_in_2);

                    // TODO: Perspective divide
                    triangle(v_out_0, v_out_1, v_out_2, shader);
                }
                break;
            }

        }

    private:
        bool target_buffer_check();

        glm::ivec2 discretize(glm::vec2 vec);

        glm::u8vec3 discretize(glm::vec3 vec);

        Buffer2D<3, unsigned char> *target_buffer;
    };

}
