#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#include <vector>
#include <utility>

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
