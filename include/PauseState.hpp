#pragma once

#include "State.hpp"
#include "Container.hpp"

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time deltaTime);
	virtual bool handleEvent(const sf::Event& event);
private:
	void addButton(Context context, float y, const std::string& text, std::function<void()> callback);
	GUI::Container mGUIContainer;
};