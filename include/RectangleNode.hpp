#pragma once

#include "SceneNode.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

class RectangleNode : public SceneNode
{
public:
	explicit RectangleNode(sf::FloatRect rect, sf::Color color);
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape mShape;
};