#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <set>

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

typedef boost::shared_ptr<sf::Drawable> drawablePtr;
class renderer
{
    private:

        typedef std::multiset<drawablePtr> drawableList;

        drawableList _early, _main, _late;

    public:

        void addEarly(const drawablePtr &d);
        void addMain(const drawablePtr &d);
        void addLate(const drawablePtr &d);
        void removeEarly(const drawablePtr &d);
        void removeMain(const drawablePtr &d);
        void removeLate(const drawablePtr &d);

        void render(sf::RenderTarget &rt);
};

#endif // RENDERER_HPP_INCLUDED
