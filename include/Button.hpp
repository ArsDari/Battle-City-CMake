#pragma once

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{
	class Button : public Component
	{
	public:
		using Ptr = std::shared_ptr<Button>;
		using Callback = std::function<void()>;
		Button(const FontHolder& fonts, const TextureHolder& textures);
		void setCallback(Callback callback);
		void setText(const std::string& text);
		void setToggle(bool flag);
		void setPosition(float x, float y);
		virtual void setShowSprite(bool flag);
		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();
		virtual void activate();
		virtual void deactivate();
		virtual void handleEvent(const sf::Event& event);
		virtual void update(sf::Time deltaTime);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		Callback mCallback;
		sf::Sprite mSprite;
		sf::Text mText;
		sf::Time mAnimationTime;
		bool mAnimationTick;
		bool mShowSprite;
		bool mManuallyShowSprite;
		bool mIsToggle;
	};
}