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
        virtual gridPoint gridPosition() const;
        virtual void gridPosition(const gridPoint &gp);
};

class dummyEntity: public entity
{
    public:

        dummyEntity(world &w):
            entity(w)
        {
        }

        bool tick(float dt) { return false; }
        vec2 position() const { return vec2(0.0f, 0.0f); }
        void position(const vec2 &p) {};
};

typedef boost::shared_ptr<entity> entityPtr;

#endif // ENTITY_HPP_INCLUDED
