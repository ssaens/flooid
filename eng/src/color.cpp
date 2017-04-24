//
// Created by Dillon Yao on 4/23/17.
//

#include "color.h"

namespace eng {

    std::ostream &operator<<(std::ostream &os, const Color &c) {
        os << "(r: " << c.r;
        os << ", g: " << c.g;
        os << ", b: " << c.b;
        os << ", a: " << c.a;
        os << ")" << std::endl;
        return os;
    }

}