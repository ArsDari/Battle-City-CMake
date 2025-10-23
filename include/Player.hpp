#pragma once

#include "Command.hpp"
#include "PlayerData.hpp"
#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		ActionCount
	};
	Player(Category::Type categoryType, Tank::Type tankType, sf::Vector2f spawnpoint);
	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);
	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;
	PlayerData& getPlayerData();
	PlayerData getPlayerData() const;
private:
	void initializeActions();
	static bool isRealtimeAction(Action action);
	std::map<sf::Keyboard::Key, Action> mKeyBinding;
	std::map<Action, Command> mActionBinding;
	PlayerData mPlayerData;
};