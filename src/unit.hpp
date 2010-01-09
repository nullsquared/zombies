#ifndef UNIT_HPP_INCLUDED
#define UNIT_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include <SFML/System/Vector2.hpp>

#include "types.hpp"
#include "entity.hpp"
#include "renderer.hpp"

class unit: public entity
{
    private:

        renderer &_renderer;

        drawablePtr _rect;

        float _progress;

        path _path;

        void _findPath(const gridPoint &start, const gridPoint &finish);
        bool _needNewPath();

    public:

        unit(world &w, renderer &r, const gridPoint &start);
        ~unit();

        vec2 position() const;
        void position(const vec2 &p);

        bool tick(float dt);
};

typedef boost::shared_ptr<unit> unitPtr;

#endif // UNIT_HPP_INCLUDED
