#include "Player.hpp"
#include "Tank.hpp"
#include "CommandQueue.hpp"
#include <map>
#include <string>
#include <algorithm>

Player::Player(Category::Type categoryType, Tank::Type tankType, sf::Vector2f spawnpoint) :
	mPlayerData(tankType, spawnpoint)
{
	initializeActions();
	for (auto& pair : mActionBinding)
		pair.second.category = categoryType;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (auto& pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto& pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}
	return sf::Keyboard::Unknown;
}

PlayerData& Player::getPlayerData()
{
	return mPlayerData;
}

PlayerData Player::getPlayerData() const
{
	return mPlayerData;
}

void Player::initializeActions()
{
	mActionBinding[MoveLeft].action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.moveTank(Entity::Left); });
	mActionBinding[MoveRight].action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.moveTank(Entity::Right); });
	mActionBinding[MoveUp].action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.moveTank(Entity::Up); });
	mActionBinding[MoveDown].action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.moveTank(Entity::Down); });
	mActionBinding[Fire].action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.fire(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case Player::MoveLeft:
		return true;
	case Player::MoveRight:
		return true;
	case Player::MoveUp:
		return true;
	case Player::MoveDown:
		return true;
	default:
		return false;
	}
}