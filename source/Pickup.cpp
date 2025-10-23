#include "Pickup.hpp"
#include "DataTables.hpp"
#include "Category.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "PlayerData.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures) :
	Entity(1),
	mType(type),
	mSprite(textures.get(Textures::Pickups), Table[type].textureRect),
	mShowPickup(false),
	mShowPickupTick(),
	mLifetime(sf::seconds(10.f)),
	mPoints(textures.get(Textures::Points), sf::IntRect(64, 0, 16, 16)),
	mShowPoints(sf::seconds(0.5f)),
	mIsPickedUp(false)
{

}

unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Tank& tank) const
{
	if (!mIsPickedUp)
	{
		tank.getPlayerData().modifyPoints(5);
		Table[mType].action(tank);
		mIsPickedUp = true;
	}
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!mIsPickedUp)
	{
		if (mShowPickup)
			target.draw(mSprite, states);
	}
	else
		target.draw(mPoints, states);
}

void Pickup::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	if (mIsPickedUp)
	{
		if (mShowPoints <= sf::Time::Zero)
			destroy();
		else
			mShowPoints -= deltaTime;
	}
	else
	{
		if (mLifetime <= sf::Time::Zero)
			destroy();
		else
			mLifetime -= deltaTime;
	}
	if (mShowPickupTick <= sf::Time::Zero)
	{
		mShowPickupTick = sf::seconds(12.f / 60.f);
		mShowPickup = !mShowPickup;
	}
	else
		mShowPickupTick -= deltaTime;
}