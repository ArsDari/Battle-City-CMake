#include "Animation.hpp"

Animation::Animation(const sf::Texture &texture, sf::Vector2i frameSize, std::size_t numFrames, sf::Time duration) :
    mSprite(texture, sf::IntRect({ 0, 0 }, frameSize)),
    mFrameSize(frameSize),
    mNumFrames(numFrames),
    mCurrentFrame(0),
    mDuration(duration),
    mElapsedTime(),
    mRepeat(false)
{

}

void Animation::setTexture(const sf::Texture &texture)
{
    mSprite.setTexture(texture);
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
    mFrameSize = frameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
    mNumFrames = numFrames;
}

void Animation::setDuration(sf::Time duration)
{
    mDuration = duration;
}

void Animation::setRepeating(bool flag)
{
    mRepeat = flag;
}

void Animation::restart()
{
    mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
    return mCurrentFrame >= mNumFrames;
}

void Animation::update(sf::Time deltaTime)
{
    sf::Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
    mElapsedTime += deltaTime;
    sf::Vector2i textureBounds(mSprite.getTexture().getSize());
    sf::IntRect textureRect = mSprite.getTextureRect();
    if (mCurrentFrame == 0)
    {
        textureRect = sf::IntRect({ 0, 0 }, mFrameSize);
    }
    while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mNumFrames || mRepeat))
    {
        textureRect.position.x += textureRect.size.x;
        if (textureRect.position.x + textureRect.size.x > textureBounds.x)
        {
            textureRect.position.x = 0;
            textureRect.position.y += textureRect.position.y;
        }
        mElapsedTime -= timePerFrame;
        if (mRepeat)
        {
            mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;
            if (mCurrentFrame == 0)
            {
                textureRect = sf::IntRect({ 0, 0 }, mFrameSize);
            }
        }
        else
        {
            mCurrentFrame++;
        }
    }
    mSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}