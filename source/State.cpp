#include "State.hpp"
#include "StateStack.hpp"

State::Variables::Variables(int level, int mostPoints, bool isMultiplayer, bool needToLoadNextLevel) :
	level(level),
	mostPoints(mostPoints),
	isMultiplayer(isMultiplayer),
	needToLoadNextLevel(needToLoadNextLevel),
	currentLevelStatus(LevelRunning)
{

}

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Players& players, Variables& variables) :
	window(&window),
	textures(&textures),
	fonts(&fonts),
	players(&players),
	variables(&variables)
{

}

State::State(StateStack& stack, Context context) :
	mStack(&stack),
	mContext(context)
{

}

State::~State()
{

}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}