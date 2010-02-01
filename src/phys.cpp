#include "phys.hpp"

body::body(b2World &physWorld)
{
    b2BodyDef def;
    def.allowSleep = true;
    def.awake = true;
    def.type = b2_dynamicBody;
    def.userData = this;
    def.angularDamping = 0.2f;
    def.linearDamping = 10.0f;
    _body = physWorld.CreateBody(&def);
}

body::~body()
{
    _body->GetWorld()->DestroyBody(_body);
}
