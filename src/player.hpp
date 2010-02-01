#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "entity.hpp"
#include "renderer.hpp"
#include "types.hpp"

#include "phys.hpp"

class player: public entity, public body
{

    private:

        renderer &_renderer;

        drawablePtr _rect;

        vec2 _movement;

    public:

        player(world &w, renderer &r);
        ~player();

        void preStep(float dt);
        void postStep(float dt);

        bool tick(float dt);

        vec2 position() const;
        void position(const vec2 &p);

        float rotation() const;
        void rotation(float r);

        void move(const vec2 &m);
};

typedef boost::shared_ptr<player> playerPtr;

#endif // PLAYER_HPP_INCLUDED
