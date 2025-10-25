#include "LoadingState.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

LoadingState::LoadingState(StateStack& stack, Context context) :
	State(stack, context),
	mShape(sf::Vector2f(256, 224)),
	mTimer(sf::seconds(2.f)),
	mIsLowered(false),
	mLevel(context.variables->level)
{
	mShape.setFillColor(sf::Color::Black);
	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setFillColor(sf::Color::Black);
	mText.setCharacterSize(8);
	mText.setString("STAGE  " + std::to_string(mLevel));
	mText.setPosition(97, 103);
}

void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color(127, 127, 127));
	window.draw(mShape);
	if (mIsLowered)
		window.draw(mText);
}

bool LoadingState::update(sf::Time deltaTime)
{
	if (mShape.getSize().y > 0 && mShape.getPosition().y < 112)
	{
		sf::Vector2f shapeSize = mShape.getSize();
		shapeSize.y -= 16;
		mShape.setSize(shapeSize);
		mShape.move(0, 8);
	}
	else
		mIsLowered = true;
	if (mIsLowered)
	{
		if (mTimer <= sf::Time::Zero && getContext().variables->needToLoadNextLevel)
		{
			getContext().variables->level = mLevel;
			requestStackPop();
			requestStackPush(States::Game);
		}
		else
			mTimer -= deltaTime;
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	// 5 levels for now
	int maxLevel = 5;
	if (event.type == sf::Event::KeyReleased && mIsLowered && !getContext().variables->needToLoadNextLevel)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			if (mLevel + 1 < maxLevel + 1) 
			{
				mLevel += 1;
				mText.setString("STAGE  " + std::to_string(mLevel));
			}
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			if (mLevel - 1 > 0)
			{
				mLevel -= 1;
				mText.setString("STAGE  " + std::to_string(mLevel));
			}
		}
		else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space)
		{
			getContext().variables->level = mLevel;
			requestStackPop();
			requestStackPush(States::Game);
		}
	}
	return true;
}