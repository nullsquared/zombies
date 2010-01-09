#include <SFML/Graphics/Shape.hpp>

#include "player.hpp"
#include "world.hpp"

player::player(world &w, renderer &r):
    entity(w),
    _renderer(r)
{
    type = PLAYER;

    _rect.reset(new sf::Shape(sf::Shape::Rectangle(-0.5f, -0.5f, 0.5f, 0.5f, sf::Color(128, 128, 128))));
    _renderer.addMain(_rect);
}

player::~player()
{
    _renderer.removeMain(_rect);
}

bool player::tick(float dt)
{
    vec2 p = position();
    p += _movement;
    _rect->SetPosition(p);

    _movement = vec2(0.0f, 0.0f);

    return true;
}

void player::move(const vec2 &m)
{
    _movement += m;
}

vec2 player::position() const
{
    return _rect->GetPosition();
}

void player::position(const vec2 &p)
{
    _rect->SetPosition(p);
}
