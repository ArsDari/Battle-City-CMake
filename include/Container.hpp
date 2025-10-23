#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>

namespace GUI
{
	class Container : public Component
	{
	public:
		using Ptr = std::shared_ptr<Container>;
		Container();
		void pack(Component::Ptr component);
		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& event);
		virtual void update(sf::Time deltaTime);
		virtual void setShowSprite(bool flag);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		bool hasSelection() const;
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;
	};
}