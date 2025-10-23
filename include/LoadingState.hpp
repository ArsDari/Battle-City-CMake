#pragma once

#include "State.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class LoadingState : public State
{
public:
	LoadingState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time deltaTime);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::RectangleShape mShape;
	sf::Text mText;
	sf::Time mTimer;
	bool mIsLowered;
	int mLevel;
};