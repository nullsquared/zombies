#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <utility>

#include <cmath>

typedef sf::Vector2f vec2;

inline vec2 operator *(const vec2 &a, float f)
{
    return vec2(a.x * f, a.y * f);
}

inline vec2 &operator *=(vec2 &a, float f)
{
    a.x *= f;
    a.y *= f;
    return a;
}

inline vec2 operator *(const vec2 &a, const vec2 &b)
{
    return vec2(a.x * b.x, a.y * b.y);
}

inline vec2 &operator *=(vec2 &a, const vec2 &b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

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

struct gridPoint
{
    int r, c;
    gridPoint(): r(0), c(0) {}
    gridPoint(int r, int c): r(r), c(c) {}
    bool operator <(const gridPoint &rhs) const { return r != rhs.r ? r < rhs.r : c < rhs.c; }
};

struct pathPoint: gridPoint
{
    bool teleport;
    pathPoint(bool t = false): teleport(t) {}
    pathPoint(int r, int c, bool t = false): gridPoint(r, c), teleport(t) {}
    pathPoint(const gridPoint &p, bool t = false): gridPoint(p), teleport(t) {}
};

typedef std::vector<pathPoint> path;

#endif // TYPES_HPP_INCLUDED
