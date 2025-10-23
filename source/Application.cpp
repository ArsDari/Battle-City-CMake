#include "Application.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "LoadingState.hpp"
#include "GameOverState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application() :
	mWindow(sf::VideoMode(256 * 3, 224 * 3), "Battle City"),
	mTextures(),
	mFonts(),
	mPlayers(),
	mVariables(1, 200, false, false),
	mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayers, mVariables)),
	mStatisticsText(),
	mStatisticsUpdateTime(),
	mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(Fonts::Main, "Media/Fonts/Main.otf");
	mFonts.get(Fonts::Main).setSmooth(false);
	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(4.f, 4.f);
	mStatisticsText.setCharacterSize(8u);
	loadTextures();
	createPlayers();
	registerStates();
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		timeSinceLastUpdate += deltaTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);
			if (mStateStack.isEmpty())
				mWindow.close();
		}
		updateStatistics(deltaTime);
		render();
	}
}

void Application::processInput()
{
	while (const std::optional event = mWindow.pollEvent()) {
		mStateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time deltaTime)
{
	mStateStack.update(deltaTime);
}

void Application::render()
{
	mStateStack.draw();
	mWindow.setView(sf::View(sf::Vector2f(128, 112), sf::Vector2f(256, 224)));
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Application::updateStatistics(sf::Time deltaTime)
{
	mStatisticsUpdateTime += deltaTime;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::loadTextures()
{
	mTextures.load(Textures::FirstPlayer, "Media/Tileset/Tileset.png", sf::IntRect(0, 0, 128, 64));
	mTextures.load(Textures::SecondPlayer, "Media/Tileset/Tileset.png", sf::IntRect(0, 64, 128, 64));
	mTextures.load(Textures::EnemyRegularTank, "Media/Tileset/Tileset.png", sf::IntRect(128, 0, 128, 32));
	mTextures.load(Textures::EnemyAPC, "Media/Tileset/Tileset.png", sf::IntRect(128, 32, 128, 32));
	mTextures.load(Textures::EnemyRapidFireTank, "Media/Tileset/Tileset.png", sf::IntRect(128, 64, 128, 32));
	mTextures.load(Textures::EnemyHeavyTank, "Media/Tileset/Tileset.png", sf::IntRect(128, 96, 128, 64));
	mTextures.load(Textures::Tileset, "Media/Tileset/Tileset.png", sf::IntRect(256, 64, 80, 120));
	mTextures.load(Textures::BulletTile, "Media/Tileset/Tileset.png", sf::IntRect(256, 32, 32, 16));
	mTextures.load(Textures::Pickups, "Media/Tileset/Tileset.png", sf::IntRect(256, 0, 112, 16));
	mTextures.load(Textures::InvicibilityFrames, "Media/Tileset/Tileset.png", sf::IntRect(256, 48, 32, 16));
	mTextures.load(Textures::SpawningFrames, "Media/Tileset/Tileset.png", sf::IntRect(336, 80, 32, 48));
	mTextures.load(Textures::Points, "Media/Tileset/Tileset.png", sf::IntRect(288, 48, 80, 16));
	mTextures.load(Textures::SmallExplosionFrames, "Media/Tileset/Tileset.png", sf::IntRect(256, 16, 48, 16));
	mTextures.load(Textures::BigExplosionFrames, "Media/Tileset/Tileset.png", sf::IntRect(304, 16, 64, 32));
	mTextures.load(Textures::ButtonSelected, "Media/Tileset/Tileset.png", sf::IntRect(32, 0, 32, 16));
	mTextures.load(Textures::Statistics, "Media/Tileset/Tileset.png", sf::IntRect(368, 0, 32, 224));
	mTextures.load(Textures::TankMark, "Media/Tileset/Tileset.png", sf::IntRect(336, 64, 8, 8));
	mTextures.load(Textures::Logo, "Media/Tileset/Tileset.png", sf::IntRect(0, 160, 192, 72));
	mTextures.load(Textures::FirstPlayerScorePrefix, "Media/Tileset/Tileset.png", sf::IntRect(336, 72, 16, 8));
	mTextures.load(Textures::MostScorePrefix, "Media/Tileset/Tileset.png", sf::IntRect(344, 64, 24, 8));
	mTextures.load(Textures::SecondPlayerScorePrefix, "Media/Tileset/Tileset.png", sf::IntRect(352, 72, 16, 8));
}

void Application::createPlayers() const
{
	Player firstPlayer(Category::FirstPlayerTank, Tank::FirstPlayer, sf::Vector2f(80, 200));
	firstPlayer.assignKey(Player::MoveLeft, sf::Keyboard::A);
	firstPlayer.assignKey(Player::MoveRight, sf::Keyboard::D);
	firstPlayer.assignKey(Player::MoveUp, sf::Keyboard::W);
	firstPlayer.assignKey(Player::MoveDown, sf::Keyboard::S);
	firstPlayer.assignKey(Player::Fire, sf::Keyboard::Space);
	Player secondPlayer(Category::SecondPlayerTank, Tank::SecondPlayer, sf::Vector2f(144, 200));
	secondPlayer.assignKey(Player::MoveLeft, sf::Keyboard::Left);
	secondPlayer.assignKey(Player::MoveRight, sf::Keyboard::Right);
	secondPlayer.assignKey(Player::MoveUp, sf::Keyboard::Up);
	secondPlayer.assignKey(Player::MoveDown, sf::Keyboard::Down);
	secondPlayer.assignKey(Player::Fire, sf::Keyboard::Slash);
	mPlayers.push_back(firstPlayer);
	mPlayers.push_back(secondPlayer);
}

void Application::registerStates()
{
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.pushState(States::Menu);
}