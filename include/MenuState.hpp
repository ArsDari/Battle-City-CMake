#pragma once

#include "TileNode.hpp"
#include "State.hpp"
#include "Container.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <functional>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time deltaTime);
	virtual bool handleEvent(const sf::Event& event);
private:
	void pack();
	void addButton(Context context, float y, const std::string& text, std::function<void()> callback);
	std::string setStringPoints(int number) const;
	sf::Sprite mLogo;
	sf::View mView;
	GUI::Container mGUIContainer;
	bool mIsMenuRolledOut;
};