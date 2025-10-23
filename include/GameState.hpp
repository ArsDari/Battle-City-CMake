#pragma once

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time deltaTime);
	virtual bool handleEvent(const sf::Event& event);
	void setLevelStatus(LevelStatus status);
	LevelStatus getLevelStatus() const;
private:
	World mWorld;
	Players& mPlayers;
	LevelStatus mCurrentLevelStatus;
	sf::RectangleShape mUpperShape;
	sf::RectangleShape mLowerShape;
	sf::Time mGameOverTimer;
	bool mHasLevelEnded;
	bool mIsGameOver;
};