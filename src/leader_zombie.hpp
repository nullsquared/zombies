#ifndef LEADER_ZOMBIE_HPP
#define LEADER_ZOMBIE_HPP

#include <boost/shared_ptr.hpp>

#include <set>

#include <SFML/System/Vector2.hpp>

#include "types.hpp"
#include "entity.hpp"
#include "renderer.hpp"
#include "npc.hpp"

#include "phys.hpp"

class leaderZombie: public npc, public body
{
    private:

        renderer &_renderer;

        drawablePtr _rect;

        path _path;

        vec2 _movement;

        std::set<gridPoint> _explored;

        bool _findPath(const gridPoint &start, const gridPoint &finish);
        bool _canExplore(const gridPoint &p);
        void _lookAround();

    public:

        leaderZombie(world &w, renderer &r);
        ~leaderZombie();

        void preStep(float dt);
        void postStep(float dt);

        vec2 position() const;
        void position(const vec2 &p);

        float rotation() const;
        void rotation(float r);

        bool tick(float dt);
};

typedef boost::shared_ptr<leaderZombie> leaderZombiePtr;

#endif // LEADER_ZOMBIE_HPP
