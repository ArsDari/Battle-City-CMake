#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	explicit Animation(const sf::Texture& texture, sf::Vector2i frameSize, std::size_t numFrames, sf::Time duration);
	void setTexture(const sf::Texture& texture);
	void setFrameSize(sf::Vector2i frameSize);
	void setNumFrames(std::size_t numFrames);
	void setDuration(sf::Time duration);
	void setRepeating(bool flag);
	void restart();
	bool isFinished() const;
	void update(sf::Time deltaTime);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Sprite mSprite;
	sf::Vector2i mFrameSize;
	std::size_t mNumFrames;
	std::size_t mCurrentFrame;
	sf::Time mDuration;
	sf::Time mElapsedTime;
	bool mRepeat;
};