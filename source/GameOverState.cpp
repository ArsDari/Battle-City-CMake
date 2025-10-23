#include "GameOverState.hpp"
#include "Player.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

GameOverState::GameOverState(StateStack& stack, Context context) :
	State(stack, context),
	mElapsedTime(),
	mFinishedCounting(false)
{
	mPointsTable[Tank::EnemyRegularTank] = 1;
	mPointsTable[Tank::EnemyAPC] = 2;
	mPointsTable[Tank::EnemyRapidFireTank] = 3;
	mPointsTable[Tank::EnemyHeavyTank] = 4;
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear();
}

bool GameOverState::update(sf::Time deltaTime)
{
	for (auto& player : *getContext().players)
	{
		PlayerData::Statistics statistics = player.getPlayerData().resetStatistics();
		for (auto& pair : statistics)
		{
			for (int i = 0; i < pair.second; ++i)
			{
				player.getPlayerData().modifyPoints(mPointsTable[pair.first]);
				// expand and rework this in new version
			}
		}
		mFinishedCounting = true;
	}
	if (mFinishedCounting)
	{
		if (getContext().variables->currentLevelStatus == LevelFailure)
		{
			requestStackClear();
			requestStackPush(States::Menu);
		}
		else
		{
			int& level = getContext().variables->level;
			// 5 levels for now 
			level = level + 1 > 5 ? 1 : level + 1;
			getContext().variables->needToLoadNextLevel = true;
			requestStackClear();
			requestStackPush(States::Loading);
		}
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	return false;
}