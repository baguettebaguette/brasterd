#pragma once

#include "Buffer.h"
#include <iostream>


namespace brasterd {

    template<typename T>
    void swap(T &a, T &b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    Buffer2D<4, float> read_image(std::string path);

}
