#include <SFML/Graphics/Shape.hpp>

#include "portal.hpp"
#include "world.hpp"

portal::portal(world &w, renderer &r):
    entity(w),
    _renderer(r),
    connection(NULL)
{
    type = PORTAL;

    _rect.reset(new sf::Shape(sf::Shape::Rectangle(-0.5f, -0.5f, 0.5f, 0.5f, sf::Color(0, 0, 230))));
    _renderer.addMain(_rect);
}

portal::~portal()
{
    _renderer.removeMain(_rect);
}

bool portal::tick(float dt)
{
    return true;
}

vec2 portal::position() const
{
    return _rect->GetPosition();
}

void portal::position(const vec2 &p)
{
    _rect->SetPosition(p);
}

float portal::rotation() const
{
    return _rect->GetRotation();
}

void portal::rotation(float r)
{
    _rect->SetRotation(r);
}
