#include "Animation.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

Animation::Animation() :
	mSprite(),
	mFrameSize(),
	mNumFrames(0),
	mCurrentFrame(0),
	mDuration(),
	mElapsedTime(),
	mRepeat(false)
{

}

Animation::Animation(const sf::Texture& texture, sf::Vector2i frameSize, std::size_t numFrames, sf::Time duration) :
	mSprite(texture),
	mFrameSize(frameSize),
	mNumFrames(numFrames),
	mCurrentFrame(0),
	mDuration(duration),
	mElapsedTime(),
	mRepeat(false)
{
	mSprite.setTextureRect(sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y));
}

void Animation::setTexture(const sf::Texture& texture)
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
	sf::Vector2i textureBounds(mSprite.getTexture()->getSize());
	sf::IntRect textureRect = mSprite.getTextureRect();
	if (mCurrentFrame == 0)
		textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
	while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mNumFrames || mRepeat))
	{
		textureRect.left += textureRect.width;
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}
		mElapsedTime -= timePerFrame;
		if (mRepeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;
			if (mCurrentFrame == 0)
				textureRect = sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y);
		}
		else
			mCurrentFrame++;
	}
	mSprite.setTextureRect(textureRect);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}