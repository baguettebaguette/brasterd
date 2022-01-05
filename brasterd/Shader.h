#pragma once

#include <functional>
#include <map>
#include <glm/glm.hpp>


namespace brasterd {

    template<int v_ch_in, int v_ch_out, int f_ch_out = 4>
    class Shader {
    public:
        Shader(std::function<glm::vec<v_ch_out, float>(glm::vec<v_ch_in, float>)> vertex_shader,
            std::function<glm::vec<f_ch_out, float>(glm::vec<v_ch_out, float>)> fragment_shader) {
            this->vertex_shader = vertex_shader;
            this->fragment_shader = fragment_shader;
        }

        ~Shader() {

        }

    private:
        std::function<glm::vec<v_ch_out, float>(glm::vec<v_ch_in, float>)> vertex_shader;
        std::function<glm::vec<f_ch_out, float>(glm::vec<v_ch_out, float>)> fragment_shader;
        friend class Renderer;
    };

}


