#include <SFML/Graphics/Shape.hpp>

#include "player.hpp"
#include "world.hpp"

#include "phys.hpp"

player::player(world &w, renderer &r):
    entity(w),
    body(w.phys()),
    _renderer(r)
{
    type = PLAYER;

    _rect.reset(new sf::Shape(sf::Shape::Rectangle(-0.5f, -0.5f, 0.5f, 0.5f, sf::Color(128, 128, 128))));
    _renderer.addMain(_rect);

    b2CircleShape shape;
    shape.m_radius = 0.5f;
    shape.m_type = b2Shape::e_circle;
    _body->CreateFixture(&shape, 1.0f);
}

player::~player()
{
    _renderer.removeMain(_rect);
}

void player::postStep(float dt)
{
    // explicit vec2 conversion is necessary
    _rect->SetPosition(vec2(_body->GetPosition()));
}

void player::preStep(float dt)
{
    vec2 curVel = _body->GetLinearVelocity();
    vec2 desiredVel = _movement;
    vec2 accel = desiredVel - curVel;
    vec2 force = accel * _body->GetMass();
    _body->ApplyForce(force / dt, _body->GetWorldCenter());
}

bool player::tick(float dt)
{
    // clear movement vector
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
    _body->SetTransform(p, 0);
    _rect->SetPosition(p);
}

float player::rotation() const
{
    return _rect->GetRotation();
}

void player::rotation(float r)
{
    _rect->SetRotation(r);
}
