#pragma once

#include <functional>
#include <map>
#include <glm/glm.hpp>


namespace brasterd {

    template<int v_ch_in, int v_ch_out, int f_ch_out = 4>
    class Shader {
    public:
        Shader(std::function<Attribs<v_ch_out>(Attribs<v_ch_in>)> vertex_shader,
            std::function<Attribs<f_ch_out>(Attribs<v_ch_out>)> fragment_shader) {
            this->vertex_shader = vertex_shader;
            this->fragment_shader = fragment_shader;
        }

        ~Shader() {

        }

    private:
        std::function<Attribs<v_ch_out>(Attribs<v_ch_in>)> vertex_shader;
        std::function<Attribs<f_ch_out>(Attribs<v_ch_out>)> fragment_shader;
        friend class Renderer;
    };

    

}


