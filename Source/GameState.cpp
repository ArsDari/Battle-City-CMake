#include "GameState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),
	mWorld(*context.textures, *context.fonts, *context.players, context.variables->isMultiplayer, context.variables->level),
	mPlayers(*context.players),
	mCurrentLevelStatus(LevelRunning),
	mUpperShape(sf::Vector2f(240, 104)),
	mLowerShape(sf::Vector2f(240, 104)),
	mGameOverTimer(sf::seconds(5.f)),
	mHasLevelEnded(false),
	mIsGameOver(false)
{
	mUpperShape.setFillColor(sf::Color(127, 127, 127));
	mLowerShape.setFillColor(sf::Color(127, 127, 127));
	mUpperShape.setPosition(16, 8);
	mLowerShape.setPosition(16, 112);
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color(127, 127, 127));
	mWorld.draw(window);
	window.draw(mUpperShape);
	window.draw(mLowerShape);
}

bool GameState::update(sf::Time deltaTime)
{
	if (mUpperShape.getSize().y > 0 && mLowerShape.getSize().y > 0)
	{
		sf::Vector2f shapeSize = mUpperShape.getSize();
		shapeSize.y -= 8;
		mUpperShape.setSize(shapeSize);
		mLowerShape.setSize(shapeSize);
		mLowerShape.move(0, 8);
	}
	else
		mWorld.update(deltaTime);
	if (!mIsGameOver)
	{
		if (mWorld.numberOfAlivePlayers() == 0 || mWorld.isBaseDestroyed())
		{
			setLevelStatus(LevelFailure);
			mHasLevelEnded = true;
			if (!mIsGameOver)
			{
				mGameOverTimer = sf::seconds(5.f);
				mIsGameOver = true;
			}
		}
		else if (mWorld.hasAllActiveEnemiesDied() && !mWorld.hasRemainingEnemies())
		{
			setLevelStatus(LevelSuccess);
			mHasLevelEnded = true;
		}
	}
	if (mHasLevelEnded)
	{
		if (mGameOverTimer <= sf::Time::Zero)
		{
			getContext().variables->currentLevelStatus = getLevelStatus();
			requestStackClear();
			requestStackPush(States::GameOver);
		}
		else
			mGameOverTimer -= deltaTime;
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	if (getLevelStatus() != LevelFailure)
	{
		for (auto& player : mPlayers)
			player.handleRealtimeInput(commands);
	}
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();
	if (getLevelStatus() != LevelFailure)
	{
		for (auto& player : mPlayers)
			player.handleEvent(event, commands);
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && getLevelStatus() == LevelRunning)
		requestStackPush(States::Pause);
	return true;
}

void GameState::setLevelStatus(LevelStatus status)
{
	mCurrentLevelStatus = status;
}

GameState::LevelStatus GameState::getLevelStatus() const
{
	return mCurrentLevelStatus;
}