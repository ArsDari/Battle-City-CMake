#include "MenuState.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

static int countDigit(int number)
{
	if (number < 1)
		return 1;
	return int(log10(number) + 1);
}

MenuState::MenuState(StateStack& stack, Context context) :
	State(stack, context),
	mLogo(context.textures->get(Textures::Logo)),
	mView(context.window->getView()),
	mGUIContainer(),
	mIsMenuRolledOut(false)
{
	if (context.variables->mostPoints > 9999)
		context.variables->mostPoints %= 10000;
	mLogo.setPosition(24, 40);
	mView.move(0, -mView.getSize().y);
	pack();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear();
	window.setView(mView);
	window.draw(mLogo);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time deltaTime)
{
	mGUIContainer.update(deltaTime);
	if (!mIsMenuRolledOut)
	{
		mView.move(0, 1);
		if (mView.getCenter().y >= 112)
			mIsMenuRolledOut = true;
	}
	else
	{
		mGUIContainer.setShowSprite(true);
		mView.setCenter(128, 112);
	}
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (mIsMenuRolledOut)
		mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::KeyReleased)
		mIsMenuRolledOut = true;
	return false;
}

void MenuState::pack()
{
	Context context = getContext();
	addButton(context, 124, "1 PLAYER", [this]() {
		(*getContext().players)[0].getPlayerData().reset();
		(*getContext().players)[1].getPlayerData().clear();
		getContext().variables->level = 1;
		getContext().variables->isMultiplayer = false;
		getContext().variables->needToLoadNextLevel = false;
		requestStackPop();
		requestStackPush(States::Loading);
		});
	addButton(context, 140, "2 PLAYERS", [this]() {
		(*getContext().players)[0].getPlayerData().reset();
		(*getContext().players)[1].getPlayerData().reset();
		getContext().variables->level = 1;
		getContext().variables->isMultiplayer = true;
		getContext().variables->needToLoadNextLevel = false;
		requestStackPop();
		requestStackPush(States::Loading);
		});
	addButton(context, 156, "CONSTRUCTION", [this]() {
		//requestStackPop();
		//requestStackPush(States::Construction);
		});
	addButton(context, 172, "SETTINGS", [this]() {
		requestStackPush(States::Settings);
		});
	addButton(context, 188, "EXIT", [this]() {
		requestStackPop();
		});
	auto firstPlayerPointsLabel = std::make_shared<GUI::Label>("I-" + setStringPoints((*context.players)[0].getPlayerData().getPoints()), *context.fonts, false);
	firstPlayerPointsLabel->setPosition(17, 15);
	auto mostPointsLabel = std::make_shared<GUI::Label>("HI-" + setStringPoints(context.variables->mostPoints), *context.fonts, false);
	mostPointsLabel->setPosition(89, 15);
	auto secondPlayerPointsLabel = std::make_shared<GUI::Label>("I-" + setStringPoints((*context.players)[1].getPlayerData().getPoints()), *context.fonts, false);
	secondPlayerPointsLabel->setPosition(169, 15);
	mGUIContainer.pack(firstPlayerPointsLabel);
	mGUIContainer.pack(mostPointsLabel);
	if (getContext().variables->isMultiplayer)
		mGUIContainer.pack(secondPlayerPointsLabel);
}

void MenuState::addButton(Context context, float y, const std::string& text, std::function<void()> callback)
{
	auto button = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	button->setPosition(64, y);
	button->setText(text);
	button->setCallback(callback);
	mGUIContainer.pack(button);
}

std::string MenuState::setStringPoints(int number) const
{
	if (number == 0)
		return std::string(5 - countDigit(number), ' ') + "00";
	else
		return std::string(4 - countDigit(number), ' ') + std::to_string(number) + "00";
}