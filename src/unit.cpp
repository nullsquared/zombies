#include <SFML/Graphics/Shape.hpp>

#include "unit.hpp"
#include "world.hpp"
#include "types.hpp"

#include <iostream>

unit::unit(world &w, renderer &r, const gridPoint &start):
    entity(w),
    _renderer(r),
    _progress(0.0f)
{
    _rect.reset(new sf::Shape(sf::Shape::Rectangle(0.0f, 0.0f, 1.0f, 1.0f, sf::Color(255, 32, 32))));
    _renderer.addMain(_rect);
    _rect->SetPosition((float)start.c, (float)start.r);
}

unit::~unit()
{
    _renderer.removeMain(_rect);
}

bool unit::_needNewPath()
{
    return _path.size() < 2 || _progress > 1.0f;
}

bool unit::tick(float dt)
{
    _progress += dt * 7.5f;

    if (_needNewPath())
    {
        if (_path.size() >= 2)
        {
            _world.getTile(_path[0])->leave(this);
            _world.getTile(_path[1])->leave(this);
        }
        // find destination
        world::entityList::iterator i = _world.beginEntity(entity::PLAYER);
        world::entityList::iterator end = _world.endEntity(entity::PLAYER);
        entityPtr target = *i;
        _findPath(gridPosition(), target->gridPosition());

        if (_path.size() < 2)
            return true; // no good path, stay stationary
        else
        {
            _world.getTile(_path[0])->enter(this);
            _world.getTile(_path[1])->enter(this);
        }
    }

    const pathPoint &a = _path[0], &b = _path[1];

    if (b.teleport)
        _progress = 1.0f; // don't interpolate when it comes to teleporting paths

    vec2 pa((float)a.c, (float)a.r), pb((float)b.c, (float)b.r);

    _rect->SetPosition(pa * (1.0f - _progress) + pb * _progress);

    return true;
}

void unit::_findPath(const gridPoint &start, const gridPoint &finish)
{
    _path.clear();
    _progress = 0.0f;
    _world.findPath(start, finish, _path);
}

vec2 unit::position() const
{
    return _rect->GetPosition();
}

void unit::position(const vec2 &p)
{
    _rect->SetPosition(p);
}
