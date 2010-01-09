#include <list>

#include <cmath>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "types.hpp"
#include "unit.hpp"
#include "portal.hpp"
#include "player.hpp"
#include "leader_zombie.hpp"

const std::string ROOT_DIR("../../");
const std::string DATA_DIR(ROOT_DIR + "data/");

portalPtr portalA, portalB;
playerPtr playerA;

game::game():
    _window(sf::VideoMode(640, 640, 32), "path finding"),
    _cameraPos(0.0f, 0.0f),
    _cameraSize(32.0f, 32.0f),
    _leftDrawing(tile::NUM_TYPES),
    _rightDrawing(tile::NUM_TYPES),
    _running(true),
    _drawGrid(false)
{
    _world.resizeGrid(gridPoint(500, 500));

    portalA.reset(new portal(_world, _renderer));
    portalB.reset(new portal(_world, _renderer));
    _world.addEntity(portalA);
    _world.addEntity(portalB);
    portalA->connection = portalB.get();
    portalB->connection = portalA.get();

    playerA.reset(new player(_world, _renderer));
    _world.addEntity(playerA);

    _world.addEntity(leaderZombiePtr(new leaderZombie(_world, _renderer)));

    _updateView();
    _window.SetView(_view);
}

game::~game()
{
    // clear entities before the renderer is destroyed
    portalA.reset();
    portalB.reset();
    playerA.reset();
    _world.clearEntities();
}

gridPoint game::_mouseP() const
{
    vec2 p(_window.GetInput().GetMouseX() / (float)_window.GetWidth(),
           _window.GetInput().GetMouseY() / (float)_window.GetHeight());
    gridPoint ret(int(p.y * _cameraSize.y + _cameraPos.y),
                  int(p.x * _cameraSize.x + _cameraPos.x));
    _world.putInBounds(ret);
    return ret;
}

void game::_keyDown(sf::Key::Code key)
{
    switch (key)
    {
        case sf::Key::Escape:
            _running = false;
        break;

        case sf::Key::Q:
            portalA->gridPosition(_mouseP());
        break;

        case sf::Key::E:
            portalB->gridPosition(_mouseP());
        break;

        case sf::Key::G:
            _drawGrid = !_drawGrid;
        break;

        case sf::Key::F5:
            _world.saveGrid(DATA_DIR + "test_grid.txt");
        break;

        case sf::Key::F6:
            _world.loadGrid(DATA_DIR + "test_grid.txt");
        break;

        case sf::Key::Num1:
            _cameraSize *= 2.0f;
            _updateView();
        break;

        case sf::Key::Num2:
            _cameraSize *= 0.5f;
            _updateView();
        break;

        case sf::Key::F8:
        {
            sf::Image img = _window.Capture();
            img.SaveToFile(ROOT_DIR + "screenshot.jpg");
        }
        break;
    }
}

void game::_updateView()
{
    _view.SetFromRect(sf::FloatRect(
        _cameraPos.x, _cameraPos.y,
        _cameraPos.x + _cameraSize.x, _cameraPos.y + _cameraSize.y));
}

void game::_keyUp(sf::Key::Code key)
{
}

void game::_mouseDown(sf::Mouse::Button button)
{
    switch (button)
    {
        case sf::Mouse::Left:
            _leftDrawing = tile::WALL;
        break;

        case sf::Mouse::Right:
            _rightDrawing = tile::PLAIN;
        break;
    }
}

void game::_mouseUp(sf::Mouse::Button button)
{
    switch (button)
    {
        case sf::Mouse::Left:
            _leftDrawing = tile::NUM_TYPES;
        break;

        case sf::Mouse::Right:
            _rightDrawing = tile::NUM_TYPES;
        break;
    }
}

void game::_mouseWheel(int x)
{
//    _cameraPos = _mouseP() * _cameraSize + _cameraPos;
//    _cameraSize *= std::pow(1.5f, -float(x));
//    _cameraPos -= _cameraSize * 0.5f;
//    _updateView();
}

int game::run()
{
    sf::Clock clock;
    float oldTime = clock.GetElapsedTime();
    while (_running)
    {
        _handleEvents();

        float dt = clock.GetElapsedTime() - oldTime;
        oldTime += dt;

        _tick(dt);

        _render();
    }
    return 0;
}

void game::_render()
{
    _window.Clear(sf::Color(230, 230, 230));

    gridPoint gridSize = _world.gridSize();

    const float W = (float)_window.GetWidth(), H = (float)_window.GetHeight();
    float w = 1.0f, h = 1.0f;//W / gridSize.c, h = H / gridSize.r;

    sf::Shape rect = sf::Shape::Rectangle(0, 0, w, h, sf::Color(32, 32, 32));

    for (int r = 0; r < gridSize.r; ++r)
    {
        for (int c = 0; c < gridSize.c; ++c)
        {
            const tilePtr &t = _world.getTile(gridPoint(r, c));

            if (t->type == tile::WALL)
            {
                rect.SetPosition(c * w, r * h);
                _window.Draw(rect);
            }
        }
    }

    if (_drawGrid)
    {
        glBegin(GL_LINES);
        glColor3b(0, 0, 0);
        for (int i = 1; i < gridSize.r; ++i)
        {
            glVertex2f(0, float(i));
            glVertex2f(float(gridSize.c), float(i));
        }

        for (int i = 1; i < gridSize.c; ++i)
        {
            glVertex2f(float(i), 0);
            glVertex2f(float(i), float(gridSize.r));
        }
        glEnd();

        {
            gridPoint gp = playerA->gridPosition();

            rect = sf::Shape::Rectangle(0, 0, w, h, sf::Color(128, 0, 128));
            rect.SetPosition((float)gp.c, (float)gp.r);
            _window.Draw(rect);
        }
    }

    _renderer.render(_window);

    _window.Display();
}

void game::_tick(float dt)
{
    gridPoint mp = _mouseP();
    tilePtr &t = _world.getTile(mp);

    if (_leftDrawing != tile::NUM_TYPES)
        t->type = _leftDrawing;
    if (_rightDrawing != tile::NUM_TYPES)
        t->type = _rightDrawing;

//    tempPath.clear();
//    _world.findPath(startP, finishP, tempPath);

    {
        vec2 m(0.0f, 0.0f);
        const float SPEED = 15.0f;
        if (_window.GetInput().IsKeyDown(sf::Key::A))
            m.x += dt * -SPEED;
        if (_window.GetInput().IsKeyDown(sf::Key::D))
            m.x += dt * SPEED;
        if (_window.GetInput().IsKeyDown(sf::Key::W))
            m.y += dt * -SPEED;
        if (_window.GetInput().IsKeyDown(sf::Key::S))
            m.y += dt * SPEED;
        playerA->move(m);
    }

    _world.tick(dt);

    // update the world view after the player has been ticked
    {
        vec2 p = playerA->position();
        const float PAD = _cameraSize.x * 0.2f;
        _cameraPos.x = std::min(_cameraPos.x, p.x - PAD);
        _cameraPos.y = std::min(_cameraPos.y, p.y - PAD);
        _cameraPos.x = std::max(_cameraPos.x, p.x - _cameraSize.x + 1 + PAD);
        _cameraPos.y = std::max(_cameraPos.y, p.y - _cameraSize.y + 1 + PAD);
        _updateView();
    }
}

void game::_handleEvents()
{
    sf::Event e;
    while (_window.GetEvent(e))
    {
        switch (e.Type)
        {
            case sf::Event::Closed:
                _running = false;
            break;

            case sf::Event::KeyPressed:
                _keyDown(e.Key.Code);
            break;

            case sf::Event::KeyReleased:
                _keyUp(e.Key.Code);
            break;

            case sf::Event::MouseButtonPressed:
                _mouseDown(e.MouseButton.Button);
            break;

            case sf::Event::MouseButtonReleased:
                _mouseUp(e.MouseButton.Button);
            break;

            case sf::Event::MouseWheelMoved:
                _mouseWheel(e.MouseWheel.Delta);
            break;
        }
    }
}
