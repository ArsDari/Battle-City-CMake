#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "Tilemap.hpp"
#include "Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>

class PlayerData;

class Projectile : public Entity
{
public:
	enum Type
	{
		FirstPlayerSlowBullet,
		FirstPlayerFastBullet,
		SecondPlayerSlowBullet,
		SecondPlayerFastBullet,
		EnemySlowBullet,
		EnemyFastBullet,
		TypeCount
	};
	Projectile(Type type, const TextureHolder& textures, Tilemap& tilemap, Rotation rotation, PlayerData* playerData, int& bullets);
	Projectile(Type type, const TextureHolder& textures, Tilemap& tilemap, Rotation rotation, int& bullets);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;
	void remove();
	void destroy();
	int getDamage() const;
	PlayerData* getPlayerData();
private:
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	bool checkNextPosition(sf::Vector2f velocity);
	void setBrickModifierTile(sf::Vector2f& position, sf::Vector2f& size, Tilemap::Modifier modifier);
	Type mType;
	sf::Sprite mSprite;
	Tilemap& mTilemap;
	Rotation mRotation;
	int mGrade;
	PlayerData* mPlayerData;
	int& mBullets;
	sf::Vector2f mVelocity;
	Animation mExplosion;
	bool mShowExplosion;
};