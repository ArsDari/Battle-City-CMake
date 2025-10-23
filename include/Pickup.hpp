#pragma once

#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Tank;

class Pickup : public Entity
{
public:
	enum Type
	{
		Helmet,
		Clock,
		Shovel,
		Star,
		Grenade,
		Life,
		TypeCount
	};
	Pickup(Type type, const TextureHolder& textures);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	void apply(Tank& tank) const;
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	Type mType;
	sf::Sprite mSprite;
	bool mShowPickup;
	sf::Time mShowPickupTick;
	sf::Time mLifetime;
	sf::Sprite mPoints;
	sf::Time mShowPoints;
	mutable bool mIsPickedUp;
};