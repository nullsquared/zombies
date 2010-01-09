#include <boost/foreach.hpp>

#include "renderer.hpp"

void renderer::addEarly(const drawablePtr &d)
{
    _early.insert(d);
}

void renderer::addMain(const drawablePtr &d)
{
    _main.insert(d);
}

void renderer::addLate(const drawablePtr &d)
{
    _late.insert(d);
}

void renderer::removeEarly(const drawablePtr &d)
{
    _early.erase(_early.find(d));
}

void renderer::removeMain(const drawablePtr &d)
{
    _main.erase(_main.find(d));
}

void renderer::removeLate(const drawablePtr &d)
{
    _late.erase(_late.find(d));
}

void renderer::render(sf::RenderTarget &rt)
{
    BOOST_FOREACH(drawablePtr &d, _early) rt.Draw(*d);
    BOOST_FOREACH(drawablePtr &d, _main) rt.Draw(*d);
    BOOST_FOREACH(drawablePtr &d, _late) rt.Draw(*d);
}
