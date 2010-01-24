#include "npc.hpp"
#include "world.hpp"
#include "types.hpp"

entityPtr npc::nearestPlayer() const
{
    vec2 p = position();

    entityPtr ret;
    float d = 0.0f;

    world::entityList::iterator i = _world.beginEntity(entity::PLAYER),
        end = _world.endEntity(entity::PLAYER);
    for (; i != end; ++i)
    {
        const entityPtr &ent = i->second;
        vec2 ep = ent->position();
        float dd = sqlen(ep - p);
        if (!ret || d > dd)
        {
            ret = ent;
            d = dd;
        }
    }

    return ret;
}
