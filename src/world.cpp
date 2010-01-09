#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <list>
#include <fstream>
#include <set>
#include <queue>

#include <cmath>

#include <boost/foreach.hpp>

#include "world.hpp"
#include "portal.hpp"

world::world():
    _closedC(1),
    _fillC(0)
{
}

world::~world()
{
}

void world::resizeGrid(const gridPoint &p)
{
    _grid.resize(boost::extents[p.r][p.c]);
    _statusCache.resize(boost::extents[p.r][p.c]);
    _fillCache.resize(boost::extents[p.r][p.c]);
    clearGrid();
}

void world::clearGrid()
{
    gridPoint s = gridSize();

    for (int r = 0; r < s.r; ++r)
        for (int c = 0; c < s.c; ++c)
        {
            _grid[r][c].reset(new tile());
            _statusCache[r][c] = 0;
            _fillCache[r][c] = 0;
        }

    _closedC = 1;
    _fillC = 0;
}

tilePtr &world::getTile(const gridPoint &p)
{
    return _grid[p.r][p.c];
}

const tilePtr &world::getTile(const gridPoint &p) const
{
    return _grid[p.r][p.c];
}

float world::_manhattan(const gridPoint &start, const gridPoint &finish) const
{
    return float(std::abs(start.r - finish.r) + std::abs(start.c - finish.c));
}

float world::_straight(const gridPoint &start, const gridPoint &finish) const
{
    return (float)std::min(std::abs(start.r - finish.r), std::abs(start.c - finish.c));
}

float world::_heuristic(const gridPoint &start, const gridPoint &finish) const
{
    return 1.001f * _manhattan(start, finish);
//    float cost = 1.0f;
//    float scost = cost * 0.1f;
//
//    float m = _manhattan(start, finish);
//    float s = _straight(start, finish);
//
//    return scost * s;
//    return std::min(m * cost, s * scost);
//    return scost * s + cost * (m - 2.0f * s);
//    return 0.0f;
}

bool world::onGrid(const gridPoint &p) const
{
    gridPoint s = gridSize();
    return p.r >= 0 && p.c >= 0 && p.r < s.r && p.c < s.r;
}

world::entityList::iterator world::beginEntity(entity::type_t t)
{
    entityPtr dummy(new dummyEntity(*this));
    dummy->type = t;
    return _entities.lower_bound(dummy);
}

world::entityList::iterator world::endEntity(entity::type_t t)
{
    entityPtr dummy(new dummyEntity(*this));
    dummy->type = t;
    return _entities.upper_bound(dummy);
}

void world::putInBounds(gridPoint &p) const
{
    if (p.r < 0) p.r = 0;
    if (p.c < 0) p.c = 0;
    gridPoint s = gridSize();
    if (p.r >= s.r) p.r = s.r - 1;
    if (p.c >= s.c) p.c = s.c - 1;
}

void world::findPath(const gridPoint &start,
                     const gridPoint &finish,
                     path &thePath,
                     const world::exploreFunc_t &canExplore)
{
    if (canExplore && !canExplore(start) || !canExplore(finish))
        return;

    // we need to take portals into account
    // todo: make this a multimap so several portals can share a gridpoint
    typedef std::map<gridPoint, portalPtr> portalMap;
    portalMap portals;
    {
        entityList::iterator i = beginEntity(entity::PORTAL);
        entityList::iterator end = endEntity(entity::PORTAL);
        for (; i != end; ++i)
        {
            portalPtr p = boost::shared_static_cast<portal>(*i);
            portals[p->gridPosition()] = p;
        }
    }

    std::multiset<scoredGridPoint> open;
    // start with the starting point
    float startH = _heuristic(start, finish);
    open.insert(scoredGridPoint(start, 0, startH));

//    gridPoint gs = gridSize();
//    int maxNodes = gs.r * gs.c;// / 1000;

//    for (int numNodes = 0; !open.empty() && numNodes < maxNodes; ++numNodes)
    while (!open.empty())
    {
        // find the best candidate to move to
        scoredGridPoint best = *open.begin();
        open.erase(open.begin());
        if (_statusCache[best.r][best.c] == _closedC) continue; // already closed, ignore
        _statusCache[best.r][best.c] = _closedC;

        if (best == finish)
        {
            thePath.push_back(best);
            for (boost::shared_ptr<scoredGridPoint> i = best.parent; i; i = i->parent)
            {
                scoredGridPoint &sgp = *i;
                thePath.push_back(sgp);
                pathPoint &prev = thePath[thePath.size() - 2];
                if (_manhattan(sgp, prev) > 1) // teleport
                    prev.teleport = true;
            }
            std::reverse(thePath.begin(), thePath.end());
            break;
        }

        tilePtr bestTile = getTile(best);

        // add possible neighbors
        std::vector<gridPoint> neighbors;
        neighbors.push_back(gridPoint(best.r - 1, best.c));
        neighbors.push_back(gridPoint(best.r, best.c - 1));
        neighbors.push_back(gridPoint(best.r, best.c + 1));
        neighbors.push_back(gridPoint(best.r + 1, best.c));
        {
            portalMap::iterator i = portals.find(best);
            if (i != portals.end())
            {
                // there's a portal here, meaning there's another neighbor to consider
                portalPtr &p = i->second;
                if (p->connection)
                    neighbors.push_back(p->connection->gridPosition());
            }
        }
        for (size_t i = 0; i < neighbors.size(); ++i)
        {
            gridPoint &p = neighbors[i];

            if (!onGrid(p)) continue;
            if (_statusCache[p.r][p.c] == _closedC) continue;
            if (canExplore && !canExplore(p)) continue;

            tilePtr pTile = getTile(p);

            float g = best.g + pTile->cost(); // one more step to get here
            float h = _heuristic(p, finish);
            {
                // we have to make sure the heuristic takes into acount portals
                float mtoportal = std::numeric_limits<float>::max();
                float mfromportal = mtoportal;
                BOOST_FOREACH(portalMap::value_type &v, portals)
                {
                    portalPtr &p = v.second;
                    mtoportal = std::min(_heuristic(p->gridPosition(), best), mtoportal);
                    mfromportal = std::min(_heuristic(p->gridPosition(), finish), mfromportal);
                }
                float hportals = mtoportal + mfromportal - 1;
                h = std::min(h, hportals);
            }

            scoredGridPoint sgp(p, g, h);
            sgp.parent.reset(new scoredGridPoint(best));
            open.insert(scoredGridPoint(sgp));
        }
    }

    _closedC++;
}

gridPoint world::gridSize() const
{
    size_t numR = _grid.shape()[0], numC = _grid.shape()[1];
    return gridPoint((int)numR, (int)numC);
}

void world::saveGrid(const std::string &fn)
{
    gridPoint s = gridSize();

    std::ofstream out(fn.c_str());

    for (int r = 0; r < s.r; ++r)
    {
        for (int c = 0; c < s.c; ++c)
        {
            tilePtr &t = _grid[r][c];
            out << (int)t->type;
        }
        out << "\n";
    }
}

void world::loadGrid(const std::string &fn)
{
    std::ifstream in(fn.c_str());

    std::vector<std::string> lines;

    size_t numC = 0;
    for (size_t r = 0; !in.eof(); ++r)
    {
        std::string line;
        std::getline(in, line);
        if (line.empty())
            continue;
        lines.push_back(line);
        numC = std::max(numC, line.size());
    }
    size_t numR = lines.size();

    resizeGrid(gridPoint(numR, numC));

    for (size_t r = 0; r < numR; ++r)
    {
        for (size_t c = 0; c < lines[r].size(); ++c)
        {
            _grid[r][c]->type = (tile::type_t)(lines[r][c] - '0');
        }
    }
}

void world::addEntity(const entityPtr &e)
{
    _entities.insert(e);
}

void world::removeEntity(const entityPtr &e)
{
    _entities.erase(_entities.find(e));
}

void world::tick(float dt)
{
    for (entityList::iterator i = _entities.begin(); i != _entities.end(); )
    {
        entityPtr &e = *i;
        if (!e->tick(dt))
            i = _entities.erase(i);
        else
            ++i;
    }
}
