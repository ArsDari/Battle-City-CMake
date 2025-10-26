#include "Application.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"
#include "LoadingState.hpp"
#include "GameOverState.hpp"

constexpr unsigned int WINDOW_WIDTH = 256;
constexpr unsigned int WINDOW_HEIGHT = 224;
constexpr unsigned int WINDOW_SIZE_MULTIPLIER = 3;
constexpr sf::Vector2u WINDOW_SIZE{ WINDOW_WIDTH, WINDOW_HEIGHT };

constexpr sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application() :
    mWindow(
        sf::VideoMode(WINDOW_SIZE *WINDOW_SIZE_MULTIPLIER),
        "Battle City",
        sf::Style::Close,
        sf::State::Windowed),
    mTextures(),
    mFonts(),
    mPlayers(),
    mVariables(1, 200, false, false),
    mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayers, mVariables))
{
    mWindow.setKeyRepeatEnabled(false);
    loadMedia();
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
            {
                mWindow.close();
            }
        }
        render();
    }
}

void Application::processInput()
{
    while (const std::optional event = mWindow.pollEvent())
    {
        mStateStack.handleEvent(event);
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close();
        }
    }
}

void Application::update(sf::Time deltaTime)
{
    mStateStack.update(deltaTime);
}

void Application::render()
{
    mStateStack.draw();
    mWindow.setView(sf::View(sf::Vector2f(WINDOW_SIZE / 2U), sf::Vector2f(WINDOW_SIZE)));
    mWindow.display();
}

void Application::loadMedia()
{
    mFonts.load(Fonts::Main, "Media/Fonts/Main.otf");
    mFonts.get(Fonts::Main).setSmooth(false);
    mTextures.load(Textures::Titlescreen, "Media/Tiles/Titlescreen.png");
    mTextures.load(Textures::FirstPlayerTank, "Media/Tiles/FirstPlayerTank.png");
    mTextures.load(Textures::SecondPlayerTank, "Media/Tiles/SecondPlayerTank.png");
    mTextures.load(Textures::EnemyLightTank, "Media/Tiles/EnemyLightTank.png");
    mTextures.load(Textures::EnemyAPC, "Media/Tiles/EnemyAPC.png");
    mTextures.load(Textures::EnemyTankDestroyer, "Media/Tiles/EnemyTankDestroyer.png");
    mTextures.load(Textures::EnemyHeavyTank, "Media/Tiles/EnemyHeavyTank.png");
    mTextures.load(Textures::BrickTileset, "Media/Tiles/BrickTileset.png");
    mTextures.load(Textures::Concrete, "Media/Tiles/Concrete.png");
    mTextures.load(Textures::WaterAnimation, "Media/Tiles/WaterAnimation.png");
    mTextures.load(Textures::Leaves, "Media/Tiles/Leaves.png");
    mTextures.load(Textures::Ice, "Media/Tiles/Ice.png");
    mTextures.load(Textures::Eagle, "Media/Tiles/Eagle.png");
    mTextures.load(Textures::DestroyedEagle, "Media/Tiles/DestroyedEagle.png");
    mTextures.load(Textures::BulletTileset, "Media/Tiles/BulletTileset.png");
    mTextures.load(Textures::Pickups, "Media/Tiles/Pickups.png");
    mTextures.load(Textures::Points, "Media/Tiles/Points.png");
    mTextures.load(Textures::InvicibilityAnimation, "Media/Tiles/InvicibilityAnimation.png");
    mTextures.load(Textures::SpawnAnimation, "Media/Tiles/SpawnAnimation.png");
    mTextures.load(Textures::SmallExplosionAnimation, "Media/Tiles/SmallExplosionAnimation.png");
    mTextures.load(Textures::BigExplosionAnimation, "Media/Tiles/BigExplosionAnimation.png");
    mTextures.load(Textures::FirstPlayerStatistics, "Media/Tiles/FirstPlayerStatistics.png");
    mTextures.load(Textures::SecondPlayerStatistics, "Media/Tiles/SecondPlayerStatistics.png");
    mTextures.load(Textures::HighScoreStatistics, "Media/Tiles/HighScoreStatistics.png");
    mTextures.load(Textures::ButtonSelect, "Media/Tiles/ButtonSelect.png");
    mTextures.load(Textures::Statistics, "Media/Tiles/Statistics.png");
    mTextures.load(Textures::IconTank, "Media/Tiles/IconTank.png");
    mTextures.load(Textures::EasterEgg, "Media/Tiles/EasterEgg.png");
}

void Application::createPlayers()
{
    Player firstPlayer(Category::FirstPlayerTank, Tank::Type::FirstPlayerTank, sf::Vector2f(80, 200));
    firstPlayer.assignKey(Player::MoveLeft, sf::Keyboard::Key::A);
    firstPlayer.assignKey(Player::MoveRight, sf::Keyboard::Key::D);
    firstPlayer.assignKey(Player::MoveUp, sf::Keyboard::Key::W);
    firstPlayer.assignKey(Player::MoveDown, sf::Keyboard::Key::S);
    firstPlayer.assignKey(Player::Fire, sf::Keyboard::Key::Space);
    Player secondPlayer(Category::SecondPlayerTank, Tank::Type::SecondPlayerTank, sf::Vector2f(144, 200));
    secondPlayer.assignKey(Player::MoveLeft, sf::Keyboard::Key::Left);
    secondPlayer.assignKey(Player::MoveRight, sf::Keyboard::Key::Right);
    secondPlayer.assignKey(Player::MoveUp, sf::Keyboard::Key::Up);
    secondPlayer.assignKey(Player::MoveDown, sf::Keyboard::Key::Down);
    secondPlayer.assignKey(Player::Fire, sf::Keyboard::Key::Slash);
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