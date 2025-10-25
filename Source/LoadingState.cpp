#include "LoadingState.hpp"
#include "ResourceHolder.hpp"

LoadingState::LoadingState(StateStack &stack, Context context) :
    State(stack, context),
    mShape(sf::Vector2f(256, 224)),
    mText(context.fonts.get(Fonts::Main), "STAGE  " + std::to_string(mLevel), 8),
    mTimer(sf::seconds(2.f)),
    mIsLowered(false),
    mLevel(context.variables.level)
{
    mShape.setFillColor(sf::Color::Black);
    mText.setPosition({ 97, 103 });
    mText.setFillColor(sf::Color::Black);
}

void LoadingState::draw()
{
    sf::RenderWindow &window = getContext().window;
    window.clear(sf::Color(127, 127, 127));
    window.draw(mShape);
    if (mIsLowered)
    {
        window.draw(mText);
    }
}

bool LoadingState::update(sf::Time deltaTime)
{
    if (mShape.getSize().y > 0 && mShape.getPosition().y < 112)
    {
        sf::Vector2f shapeSize = mShape.getSize();
        shapeSize.y -= 16;
        mShape.setSize(shapeSize);
        mShape.move({ 0, 8 });
    }
    else
    {
        mIsLowered = true;
    }
    if (mIsLowered)
    {
        if (mTimer <= sf::Time::Zero && getContext().variables.needToLoadNextLevel)
        {
            getContext().variables.level = mLevel;
            requestStackPop();
            requestStackPush(States::Game);
        }
        else
        {
            mTimer -= deltaTime;
        }
    }
    return true;
}

bool LoadingState::handleEvent(const std::optional<sf::Event> &event)
{
    // 5 levels for now
    int maxLevel = 5;
    if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
    {
        if (mIsLowered && !getContext().variables.needToLoadNextLevel)
        {
            if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up)
            {
                if (mLevel + 1 < maxLevel + 1)
                {
                    mLevel += 1;
                    mText.setString("STAGE  " + std::to_string(mLevel));
                }
            }
            else if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down)
            {
                if (mLevel - 1 > 0)
                {
                    mLevel -= 1;
                    mText.setString("STAGE  " + std::to_string(mLevel));
                }
            }
            else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space)
            {
                getContext().variables.level = mLevel;
                requestStackPop();
                requestStackPush(States::Game);
            }
        }
    }
    return true;
}