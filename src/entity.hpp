#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "types.hpp"

class world;
class entity
{
    protected:

        world &_world;

    public:

        enum type_t
        {
            GENERIC, PLAYER, PORTAL
        } type;

        entity(world &w):
            _world(w),
            type(GENERIC)
        {
        }
        virtual ~entity() {}

        virtual bool tick(float dt) = 0;
        virtual vec2 position() const = 0;
        virtual void position(const vec2 &p) = 0;
        virtual void rotation(float r) = 0;
        virtual float rotation() const = 0;
        virtual gridPoint gridPosition() const;
        virtual void gridPosition(const gridPoint &gp);
};

typedef boost::shared_ptr<entity> entityPtr;

#endif // ENTITY_HPP_INCLUDED
