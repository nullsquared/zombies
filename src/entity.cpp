#include "entity.hpp"
#include "world.hpp"

gridPoint entity::gridPosition() const
{
    vec2 p = position();
    gridPoint ret(int(p.y), int(p.x));
    _world.putInBounds(ret);
    return ret;
}

void entity::gridPosition(const gridPoint &gp)
{
    // the 0.5f is to center the object
    position(vec2(float(gp.c) + 0.5f, float(gp.r) + 0.5f));
}
