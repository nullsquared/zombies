#include <SFML/Graphics/Shape.hpp>

#include "player.hpp"
#include "world.hpp"

player::player(world &w, renderer &r):
    entity(w),
    _renderer(r),
    _body(NULL)
{
    type = PLAYER;

    _rect.reset(new sf::Shape(sf::Shape::Rectangle(-0.5f, -0.5f, 0.5f, 0.5f, sf::Color(128, 128, 128))));
    _renderer.addMain(_rect);

    b2BodyDef bdef;
    bdef.isSleeping = false;
    bdef.allowSleep = false;
    _body = _world.phys().CreateBody(&bdef);
    b2CircleDef pdef;
    pdef.density = 1.0f;
    pdef.radius = 0.5f;
    _body->CreateShape(&pdef);
    _body->SetMassFromShapes();
}

player::~player()
{
    _world.phys().DestroyBody(_body);
    _renderer.removeMain(_rect);
}

bool player::tick(float dt)
{
    {
        b2Vec2 p = _body->GetPosition();
        _rect->SetPosition(vec2(p.x, p.y));
    }

//    vec2 p = position();
//    p += _movement * dt;
////    _rect->SetPosition(p);
//    position(p);

    _body->SetLinearVelocity(b2Vec2(_movement.x, _movement.y));

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
    _body->SetXForm(b2Vec2(p.x, p.y), 0);
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
