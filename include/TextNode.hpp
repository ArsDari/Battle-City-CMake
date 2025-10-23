#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <functional>

class TextNode : public SceneNode
{
public:
	using TextCommand = std::function<void(TextNode*)>;
	explicit TextNode(const FontHolder& fonts, const std::string& text);
	virtual bool isMarkedForRemoval() const;
	void setCommand(TextCommand command);
	void setString(const std::string& text);
	sf::Text& getText();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	sf::Text mText;
	sf::Time mLifeTime;
	TextCommand mCommand;
};