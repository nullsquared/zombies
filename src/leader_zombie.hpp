#ifndef LEADER_ZOMBIE_HPP
#define LEADER_ZOMBIE_HPP

#include <boost/shared_ptr.hpp>

#include <set>

#include <SFML/System/Vector2.hpp>

#include "types.hpp"
#include "entity.hpp"
#include "renderer.hpp"
#include "npc.hpp"

class leaderZombie: public npc
{
    private:

        renderer &_renderer;

        drawablePtr _rect;

        path _path;

        std::set<gridPoint> _explored;

        bool _findPath(const gridPoint &start, const gridPoint &finish);
        bool _canExplore(const gridPoint &p);

    public:

        leaderZombie(world &w, renderer &r);
        ~leaderZombie();

        vec2 position() const;
        void position(const vec2 &p);

        bool tick(float dt);
};

typedef boost::shared_ptr<leaderZombie> leaderZombiePtr;

#endif // LEADER_ZOMBIE_HPP