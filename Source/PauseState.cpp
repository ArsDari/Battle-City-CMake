#include "PauseState.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

PauseState::PauseState(StateStack& stack, Context context) :
	State(stack, context),
	mGUIContainer()
{
	auto pausedText = std::make_shared<GUI::Label>("PAUSE", *context.fonts, true);
	pausedText->getText().setFillColor(sf::Color(224, 80, 0));
	pausedText->getText().setPosition(101, 112);
	mGUIContainer.pack(pausedText);
	addButton(context, 128, "RETURN", [this]()
		{
			requestStackPop();
		});
	addButton(context, 144, "BACK TO MENU", [this]()
		{
			for (auto& player : *getContext().players)
				player.getPlayerData().resetStatistics();
			requestStackClear();
			requestStackPush(States::Menu);
		});
	mGUIContainer.setShowSprite(true);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time deltaTime)
{
	mGUIContainer.update(deltaTime);
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}

void PauseState::addButton(Context context, float y, const std::string& text, std::function<void()> callback)
{
	auto button = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	button->setPosition(76, y);
	button->setText(text);
	button->setCallback(callback);
	mGUIContainer.pack(button);
}