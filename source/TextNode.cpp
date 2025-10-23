#include "TextNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text) :
	mLifeTime(sf::seconds(5.f))
{
	mText.setFont(fonts.get(Fonts::Main));
	mText.setCharacterSize(8U);
	setString(text);
}

bool TextNode::isMarkedForRemoval() const
{
	return mLifeTime <= sf::Time::Zero;
}

void TextNode::setCommand(TextCommand command)
{
	mCommand = command;
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
}

sf::Text& TextNode::getText()
{
	return mText;
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void TextNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	mCommand(this);
	mLifeTime -= deltaTime;
}