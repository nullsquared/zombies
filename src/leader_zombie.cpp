#include "leader_zombie.hpp"

#include <SFML/Graphics/Shape.hpp>

#include "leader_zombie.hpp"
#include "world.hpp"
#include "types.hpp"

#include "renderer.hpp"

#include <iostream>

#include <boost/bind.hpp>

leaderZombie::leaderZombie(world &w, renderer &r):
    npc(w),
    _renderer(r)
{
    _rect.reset(new sf::Shape(sf::Shape::Rectangle(-0.5f, -0.5f, 0.5f, 0.5f, sf::Color(255, 32, 32))));
    _renderer.addMain(_rect);
}

leaderZombie::~leaderZombie()
{
    _renderer.removeMain(_rect);
}

void leaderZombie::_lookAround()
{
    gridPoint p = gridPosition();
    // add what we can see
    std::vector<gridPoint> neighbors;
    neighbors.push_back(gridPoint(p.r - 1, p.c));
    neighbors.push_back(gridPoint(p.r, p.c - 1));
    neighbors.push_back(gridPoint(p.r, p.c + 1));
    neighbors.push_back(gridPoint(p.r + 1, p.c));

    for (size_t i = 0; i < neighbors.size(); ++i)
    {
        gridPoint &n = neighbors[i];
        if (_world.onGrid(n))
        {
            _explored.insert(n);
//            drawablePtr sq(new sf::Shape(sf::Shape::Rectangle(0.0f, 0.0f, 0.8f, 0.8f, sf::Color(128, 32, 32, 128))));
//            sq->SetPosition((float)n.c, (float)n.r);
//            _renderer.addLate(sq);
        }
    }
}

bool leaderZombie::tick(float dt)
{
    _lookAround();

    gridPoint p = gridPosition();
    entityPtr target = nearestPlayer();
    if (!target)
        return true;

    gridPoint targetP = target->gridPosition();

    if (_path.empty() && !_findPath(p, targetP))
        return true;

    pathPoint nextP = _path[0];
    // make sure we can move onto this spot
    {
        tilePtr t = _world.getTile(nextP);
        if (t->type == tile::WALL)
        {
            // can't move there, abort the current path
            _path.clear();
            return true;
        }
    }

    // check if it's a teleport path point
    if (nextP.teleport)
    {
        gridPosition(nextP);
        _path.erase(_path.begin());
        return true;
    }

    vec2 v(nextP.c + 0.5f, nextP.r + 0.5f);

    vec2 cv = position();
    vec2 np = cv + normalize(v - cv) * dt * 15.0f;
    position(np);

    if (sqlen(np - v) < 0.1)
        _path.erase(_path.begin());

    return true;
}

bool leaderZombie::_findPath(const gridPoint &start, const gridPoint &finish)
{
    _path.clear();
    _world.findPath(start, finish, _path, boost::bind(&leaderZombie::_canExplore, this, _1));
    // erase the first step, that's where we are right now
    if (!_path.empty())
        _path.erase(_path.begin());
    return !_path.empty();
}

bool leaderZombie::_canExplore(const gridPoint &p)
{
    if (_explored.find(p) == _explored.end())
    {
        // hasn't seen this piece yet, can explore it
        return true;
    }

    // seen this piece, figure out if it's walkable
    tilePtr t = _world.getTile(p);

    return t->type != tile::WALL;
}

vec2 leaderZombie::position() const
{
    return _rect->GetPosition();
}

void leaderZombie::position(const vec2 &p)
{
    _rect->SetPosition(p);
}

float leaderZombie::rotation() const
{
    return _rect->GetRotation();
}

void leaderZombie::rotation(float r)
{
    _rect->SetRotation(r);
}

