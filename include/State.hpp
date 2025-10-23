#pragma once

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <memory>

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
	enum LevelStatus
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
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Players& players, Variables& variables);
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Players* players;
		Variables* variables;
	};
	State(StateStack& stack, Context context);
	virtual ~State();
	virtual void draw() = 0;
	virtual bool update(sf::Time deltaTime) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;
protected:
	void requestStackPush(States::ID stateID);
	void requestStackPop();
	void requestStackClear();
	Context getContext() const;
private:
	StateStack* mStack;
	Context mContext;
};