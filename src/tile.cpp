#include "tile.hpp"

void tile::enter(entity *ent)
{
    _occupants.insert(ent);
}

void tile::leave(entity *ent)
{
    _occupants.erase(_occupants.find(ent));
}

size_t tile::numOccupants() const
{
    return _occupants.size();
}

float tile::cost() const
{
    return 1.0f;
}
