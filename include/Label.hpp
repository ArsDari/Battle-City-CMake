#pragma once

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/Text.hpp>

namespace GUI
{
	class Label : public Component
	{
	public:
		using Ptr =  std::shared_ptr<Label>;
		Label(const std::string& text, const FontHolder& fonts, bool isBlink);
		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& event);
		virtual void update(sf::Time deltaTime);
		sf::Text& getText();
	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Text mText;
		sf::Time mAnimationTime;
		bool mIsBlink;
		bool mShowText;
	};
}