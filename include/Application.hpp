#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Application
{
public:
    Application();
    void run();
private:
    void processInput();
    void update(sf::Time deltaTime);
    void render();
    void loadMedia();
    void createPlayers() const;
    void registerStates();
    static const sf::Time TimePerFrame;
    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    State::Players mPlayers;
    State::Variables mVariables;
    StateStack mStateStack;
};