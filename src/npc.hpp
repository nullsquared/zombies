#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include "entity.hpp"

class npc: public entity
{
    public:

        entityPtr nearestPlayer() const;

        npc(world &w): entity(w) {}
        virtual ~npc() {}
};

typedef boost::shared_ptr<npc> npcPtr;

#endif // NPC_HPP_INCLUDED
