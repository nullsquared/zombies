#ifndef PHYS_HPP_INCLUDED
#define PHYS_HPP_INCLUDED

#include <Box2D/Box2D.h>

#include "types.hpp"
#include "vec2.hpp"

class body
{
    protected:

        b2Body *_body;

    public:

        body(b2World &physWorld);
        virtual ~body();

        // callback for forces
        virtual void preStep(float dt) {}
        // callback for position update
        virtual void postStep(float dt) {}
};

#endif // PHYS_HPP_INCLUDED
