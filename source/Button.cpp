#include "Button.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
	Button::Button(const FontHolder& fonts, const TextureHolder& textures) :
		mCallback(),
		mSprite(textures.get(Textures::ButtonSelected)),
		mText("", fonts.get(Fonts::Main), 8),
		mAnimationTime(),
		mAnimationTick(false),
		mShowSprite(false),
		mManuallyShowSprite(false),
		mIsToggle(false)
	{
		
	}

	void Button::setShowSprite(bool flag)
	{
		mManuallyShowSprite = flag;
	}

	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string& text)
	{
		mText.setString(text);
	}

	void Button::setToggle(bool flag)
	{
		mIsToggle = flag;
	}

	void Button::setPosition(float x, float y)
	{
		mSprite.setPosition(x, y);
		mText.setPosition(x + 25, y + 3);
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();
		mShowSprite = true;
	}

	void Button::deselect()
	{
		Component::deselect();
		mShowSprite = false;
	}

	void Button::activate()
	{
		Component::activate();
		if (mIsToggle)
			mShowSprite = true;
		if (mCallback)
			mCallback();
		if (!mIsToggle)
			deactivate();
	}

	void Button::deactivate()
	{
		Component::deactivate();
		if (mIsToggle)
		{
			if (isSelected())
				mShowSprite = true;
			else
				mShowSprite = false;
		}
	}

	void Button::handleEvent(const sf::Event& event)
	{

	}

	void Button::update(sf::Time deltaTime)
	{
		if (mAnimationTime <= sf::Time::Zero)
		{
			mAnimationTime = sf::seconds(4.f / 60.f);
			mAnimationTick = !mAnimationTick;
		}
		else
			mAnimationTime -= deltaTime;
		sf::IntRect textureRect(0, 0, 16, 16);
		textureRect.left = mAnimationTick? 16 : 0;
		mSprite.setTextureRect(textureRect);
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mText, states);
		if (mShowSprite && mManuallyShowSprite)
			target.draw(mSprite, states);
	}
}