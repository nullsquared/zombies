#ifndef PORTAL_HPP_INCLUDED
#define PORTAL_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "entity.hpp"
#include "renderer.hpp"
#include "types.hpp"

class portal: public entity
{

    private:

        renderer &_renderer;

        drawablePtr _rect;

    public:

        portal(world &w, renderer &r);
        ~portal();

        bool tick(float dt);

        portal *connection;

        vec2 position() const;
        void position(const vec2 &p);

        float rotation() const;
        void rotation(float r);
};

typedef boost::shared_ptr<portal> portalPtr;

#endif // PORTAL_HPP_INCLUDED
