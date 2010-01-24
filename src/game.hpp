#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "world.hpp"
#include "renderer.hpp"
#include "types.hpp"

class game
{
    private:

        sf::RenderWindow _window;
        sf::View _view;
        vec2 _cameraPos, _cameraSize;
        void _updateView();

        world _world;
        renderer _renderer;

        tile::type_t _leftDrawing, _rightDrawing;

        gridPoint _mouseP() const;
        vec2 _mousePView() const;

        bool _running;
        bool _drawGrid;

        void _handleEvents();
        void _render();
        void _tick(float dt);

        void _keyDown(sf::Key::Code key);
        void _keyUp(sf::Key::Code key);
        void _mouseDown(sf::Mouse::Button button);
        void _mouseUp(sf::Mouse::Button button);
        void _mouseWheel(int x);

    public:

        game();
        ~game();

        int run();
};

#endif // GAME_HPP_INCLUDED
