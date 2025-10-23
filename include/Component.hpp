#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>

namespace sf
{
	class Event;
}

namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		using Ptr = std::shared_ptr<Component>;
		Component();
		virtual ~Component();
		virtual bool isSelectable() const = 0;
		bool isSelected() const;
		virtual void select();
		virtual void deselect();
		virtual bool isActive() const;
		virtual void activate();
		virtual void deactivate();
		virtual void handleEvent(const sf::Event& event) = 0;
		virtual void update(sf::Time deltaTime);
		virtual void setShowSprite(bool flag);
	private:
		bool mIsSelected;
		bool mIsActive;
	};
}