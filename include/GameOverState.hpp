#pragma once

#include "State.hpp"

class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time deltaTime);
	virtual bool handleEvent(const sf::Event& event);
private:
	sf::Time mElapsedTime;
	bool mFinishedCounting;
	std::map<Tank::Type, int> mPointsTable;
};