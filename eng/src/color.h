//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_COLOR_H
#define FLOOID_COLOR_H

#include <ostream>

namespace eng {

    class Color {
    public:
        float r;
        float g;
        float b;
        float a;

        Color(float r=0, float g=0, float b=0, float a=1.0) :
            r(r), g(g), b(b), a(a) {}

        inline Color operator+(const Color &c) const {
            return Color(r + c.r, g + c.g, b + c.b, a + c.a);
        }

        inline Color& operator+=(const Color &c) {
            r += c.r; g += c.g; b += c.b; a += c.a;
            return *this;
        }

        inline Color operator*(float s) const {
            return Color(r * s, g * s, b * s, a * s);
        }

        inline Color& operator *=(float s) {
            r *= s; g *= s; b *= s; a *= s;
            return *this;
        }

        inline bool operator==(const Color &c) const {
            return r == c.r && g == c.g && b == c.b && a == c.a;
        }

        inline bool operator !=(const Color &c) const {
            return !operator==(c);
        }

    };

    inline Color operator*(float s, const Color &c) {
        return c * s;
    }

    std::ostream &operator<<(std::ostream &os, const Color &c);
}

#endif //FLOOID_COLOR_H
