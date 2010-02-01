#ifndef VEC2_HPP_INCLUDED
#define VEC2_HPP_INCLUDED

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include <Box2D/Box2D.h>

struct vec2
{
    float x, y;

    vec2(float x, float y): x(x), y(y) {}
    vec2(): x(0.0f), y(0.0f) {}
    vec2(const vec2 &v): x(v.x), y(v.y) {}

    vec2(const sf::Vector2f &v): x(v.x), y(v.y) {}
    vec2(const b2Vec2 &v): x(v.x), y(v.y) {}

    operator sf::Vector2f() const
    {
        return sf::Vector2f(x, y);
    }

    operator b2Vec2() const
    {
        return b2Vec2(x, y);
    }

    #define OP_(op) vec2 &operator op(const vec2 &v)\
    {\
        x op v.x;\
        y op v.y;\
        return *this;\
    }
    OP_(+=)
    OP_(-=)
    OP_(*=)
    OP_(/=)
    #undef OP_

    // special cases for scalars
    vec2 operator *(float f) const
    {
        return vec2(x * f, y * f);
    }

    vec2 operator /(float f) const
    {
        return *this * (1.0f / f);
    }

    vec2 &operator *=(float f)
    {
        x *= f;
        y *= f;
        return *this;
    }

    vec2 &operator /=(float f)
    {
        return operator *=(1.0f / f);
    }
};

#define OP_(op) inline vec2 operator op(const vec2 &a, const vec2 &b)\
{\
    return vec2(a.x op b.x, a.y op b.y);\
}
OP_(+)
OP_(-)
OP_(*)
OP_(/)
#undef OP_

inline float dot(const vec2 &a, const vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

inline float sqlen(const vec2 &a)
{
    return dot(a, a);
}

inline float len(const vec2 &a)
{
    return std::sqrt(sqlen(a));
}

inline vec2 normalize(const vec2 &a)
{
    return a * (1.0f / len(a));
}

inline vec2 perp(const vec2 &a)
{
    return vec2(-a.y, a.x); // CCW rotation
}

#endif // VEC2_HPP_INCLUDED
