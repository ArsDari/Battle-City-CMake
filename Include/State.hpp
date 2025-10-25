#pragma once

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <optional>

namespace sf
{
    class RenderWindow;
}

class StateStack;

class State
{
public:
    using Ptr = std::unique_ptr<State>;
    using Players = std::vector<Player>;
    enum class LevelStatus
    {
        LevelRunning,
        LevelSuccess,
        LevelFailure
    };
    struct Variables
    {
        Variables(int level, int mostPoints, bool isMultiplayer, bool needToLoadNextLevel);
        int level;
        int mostPoints;
        bool isMultiplayer;
        bool needToLoadNextLevel;
        LevelStatus currentLevelStatus;
    };
    struct Context
    {
        Context(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts, Players &players, Variables &variables);
        sf::RenderWindow &window;
        TextureHolder &textures;
        FontHolder &fonts;
        Players &players;
        Variables &variables;
    };
    State(StateStack &stack, Context context);
    virtual ~State();
    virtual void draw() = 0;
    virtual bool update(sf::Time deltaTime) = 0;
    virtual bool handleEvent(const std::optional<sf::Event> &event) = 0;
protected:
    void requestStackPush(States stateID);
    void requestStackPop();
    void requestStackClear();
    Context getContext() const;
private:
    StateStack *mStack;
    Context mContext;
};