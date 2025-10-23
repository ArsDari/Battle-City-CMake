#pragma once

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/System/Time.hpp>
#include <vector>
#include <functional>
#include <map>

namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack
{
public:
	StateStack(const StateStack&) = delete;
	StateStack& operator=(const StateStack&) = delete;

	enum Action
	{
		Push,
		Pop,
		Clear
	};
	explicit StateStack(State::Context context);
	template <typename T>
	void registerState(States::ID stateID);
	void update(sf::Time deltaTime);
	void draw();
	void handleEvent(const sf::Event& event);
	void pushState(States::ID stateID);
	void popState();
	void clearStates();
	bool isEmpty() const;
private:
	State::Ptr createState(States::ID stateID);
	void applyPendingChanges();
	struct PendingChange
	{
		explicit PendingChange(Action action, States::ID stateID = States::None);
		Action action;
		States::ID stateID;
	};
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	State::Context mContext;
	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
		{
			return State::Ptr(new T(*this, mContext));
		};
}