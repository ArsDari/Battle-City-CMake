#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>

class LoadingState : public State
{
public:
    LoadingState(StateStack &stack, Context context);
    virtual void draw();
    virtual bool update(sf::Time deltaTime);
    virtual bool handleEvent(const std::optional<sf::Event> &event);
private:
    sf::RectangleShape mShape;
    sf::Text mText;
    sf::Time mTimer;
    bool mIsLowered;
    int mLevel;
};