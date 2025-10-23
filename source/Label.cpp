#include "Label.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	Label::Label(const std::string& text, const FontHolder& fonts, bool isBlink) :
		mText(text, fonts.get(Fonts::Main), 8),
		mIsBlink(isBlink),
		mAnimationTime(),
		mShowText(true)
	{

	}

	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::handleEvent(const sf::Event&)
	{

	}

	void Label::update(sf::Time deltaTime)
	{
		if (mIsBlink)
		{
			if (mAnimationTime <= sf::Time::Zero)
			{
				mAnimationTime = sf::seconds(16.f / 60.f);
				mShowText = !mShowText;
			}
			else
				mAnimationTime -= deltaTime;
		}
	}

	sf::Text& Label::getText()
	{
		return mText;
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		if (mShowText)
			target.draw(mText, states);
	}
}