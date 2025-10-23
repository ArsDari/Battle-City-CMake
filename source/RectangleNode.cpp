#include "RectangleNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

RectangleNode::RectangleNode(sf::FloatRect rect, sf::Color color)
{
	mShape.setSize(rect.getSize());
	mShape.setPosition(rect.getPosition());
	mShape.setFillColor(color);
}

void RectangleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}