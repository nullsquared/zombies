#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <list>
#include <set>
#include <map>
#include <utility>

#include <boost/multi_array.hpp>
#include <boost/function.hpp>

#include "phys.hpp"

#include "tile.hpp"
#include "types.hpp"

#include "entity.hpp"

class world
{
    public:

        typedef std::multimap<entity::type_t, entityPtr> entityList;

        world();
        ~world();

        typedef boost::function<bool(const gridPoint&)> exploreFunc_t;

        void findPath(const gridPoint &start,
                      const gridPoint &finish,
                      path &thePath,
                      const exploreFunc_t &canExplore = exploreFunc_t());

        void resizeGrid(const gridPoint &p);
        void clearGrid();
        bool onGrid(const gridPoint &p) const;
        gridPoint gridSize() const;
        tilePtr &getTile(const gridPoint &p);
        const tilePtr &getTile(const gridPoint &p) const;

        void saveGrid(const std::string &fn);
        void loadGrid(const std::string &fn);
        void putInBounds(gridPoint &p) const;

        void addEntity(const entityPtr &e);
        void removeEntity(const entityPtr &e);
        entityList::iterator beginEntity(entity::type_t t);
        entityList::iterator endEntity(entity::type_t t);
        void clearEntities()
        {
            _entities.clear();
        }

        void tick(float dt);

        void addedObstacle(const gridPoint &p);
        void removedObstacle(const gridPoint &p);
        void checkObstacles();

        b2World &phys() { return *_physWorld; }

    private:

        boost::shared_ptr<b2World> _physWorld;
        float _physAccum;

        struct scoredGridPoint : gridPoint
        {
            float g, h;
            boost::shared_ptr<scoredGridPoint> parent;
            scoredGridPoint(int r, int c, float g, float h):
                gridPoint(r, c), g(g), h(h) {}
            scoredGridPoint(const gridPoint &p, float g, float h):
                gridPoint(p), g(g), h(h) {}
            bool operator ==(const gridPoint &rhs) const { return r == rhs.r && c == rhs.c; }
            bool operator <(const scoredGridPoint &rhs) const { return g + h < rhs.g + rhs.h; }
        };

        entityList _entities;

        tileGrid _grid;

        typedef unsigned status_t;
        boost::multi_array<status_t, 2> _statusCache;
        int _closedC;
        boost::multi_array<status_t, 2> _fillCache;
        int _fillC;

        float _manhattan(const gridPoint &start, const gridPoint &finish) const;
        float _straight(const gridPoint &start, const gridPoint &finish) const;
        float _heuristic(const gridPoint &start, const gridPoint &finish) const;
};


#endif // WORLD_HPP_INCLUDED
