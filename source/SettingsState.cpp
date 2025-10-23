#include "SettingsState.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(StateStack& stack, Context context) :
	State(stack, context),
	mGUIContainer()
{
	addButtonLabel(Player::MoveLeft, 76, "Move Left", context);
	addButtonLabel(Player::MoveRight, 92, "Move Right", context);
	addButtonLabel(Player::MoveUp, 108, "Move Up", context);
	addButtonLabel(Player::MoveDown, 124, "Move Down", context);
	addButtonLabel(Player::Fire, 140, "Fire", context);
	updateLabels();
	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	(*backButton).setPosition(64, 172);
	(*backButton).setText("BACK");
	(*backButton).setCallback([this]()
		{
			requestStackPop();
		});
	mGUIContainer.pack(backButton);
	mGUIContainer.setShowSprite(true);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear();
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time deltaTime)
{
	mGUIContainer.update(deltaTime);
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;
	Player& player = (*getContext().players)[0];
	// redo this to be able to remap controls for 2 players
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				player.assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);
	return false;
}

void SettingsState::updateLabels()
{
	// redo this to be able to remap controls for 2 players
	Player& player = (*getContext().players)[0];
	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		mBindingLabels[i]->getText().setString(toString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(48, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);
	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts, false);
	mBindingLabels[action]->setPosition(176, y + 3);
	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}