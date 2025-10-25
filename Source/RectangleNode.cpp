#include "RectangleNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

RectangleNode::RectangleNode(sf::FloatRect rect, sf::Color color)
{
    mShape.setSize(rect.size);
    mShape.setPosition(rect.position);
    mShape.setFillColor(color);
}

void RectangleNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mShape, states);
}