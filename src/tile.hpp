#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>

#include <set>

#include "types.hpp"

class entity;
class tile
{
    private:

        std::set<entity*> _occupants;

    public:

        enum type_t
        {
            PLAIN, WALL, NUM_TYPES
        } type;

        void enter(entity *ent);
        void leave(entity *ent);
        size_t numOccupants() const;

        tile(): type(PLAIN) {}
        tile(type_t type): type(type) {}

        float cost() const;
};

typedef boost::shared_ptr<tile> tilePtr;
typedef boost::multi_array<tilePtr, 2> tileGrid;

#endif // TILE_HPP_INCLUDED
