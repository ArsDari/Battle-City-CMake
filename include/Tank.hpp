#pragma once

#include "Entity.hpp"
#include "CommandQueue.hpp"
#include "ResourceIdentifiers.hpp"
#include "Tilemap.hpp"
#include "Projectile.hpp"
#include "Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>

class PlayerData;

class Tank : public Entity
{
public:
	using TankFunction = std::function<void(Tank*)>;
	using Tanks = std::vector<Tank*>;
	enum Type
	{
		FirstPlayer,
		SecondPlayer,
		EnemyRegularTank,
		EnemyAPC,
		EnemyRapidFireTank,
		EnemyHeavyTank,
		TypeCount
	};
	Tank(Type type, const TextureHolder& textures, Tilemap& tilemap, PlayerData& playerData); // player
	Tank(Type type, const TextureHolder& textures, Tilemap& tilemap, bool isBonusTank); // enemy
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;
	virtual void remove();
	virtual void destroy();
	virtual void damage(int points);
	virtual void damage(int points, PlayerData* player);
	bool isPlayer() const;
	bool isInvincible() const;
	bool isSpawned() const;
	void moveTank(Rotation rotation);
	void stopTank(sf::Time duration, bool showStoppedTank);
	void fire();
	void makeInvincible(sf::Time duration);
	void upgrade();
	void addLife();
	void addGrade();
	void setPendingCommand(const Command& command);
	void defendBase() const;
	void checkTanksAhead(const Tanks& tanks);
	void setRotation(Rotation rotation);
	void setCommand(TankFunction function);
	PlayerData& getPlayerData();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	void updateDestroyedTank(sf::Time deltaTime);
	void updateSpawning(sf::Time deltaTime);
	void updateTimers(sf::Time deltaTime);
	void checkBonusTankSprite(sf::Time deltaTime);
	void checkHeavyTankSprite(sf::Time deltaTime);
	void checkTankSliding(sf::Time deltaTime);
	void checkTankStopped(sf::Time deltaTime);
	void checkTankInvincibility(sf::Time deltaTime);
	void updateTankSprite();
	void updateMovementPattern();
	bool updateExplosion(sf::Time deltaTime);
	bool isSequenceEnded() const;
	void updatePosition();
	bool checkNextPosition(sf::Vector2f velocity);
	void checkPickupDrop(CommandQueue& commands);
	void checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands);
	void createProjectile(SceneNode& node, const TextureHolder& textures, Projectile::Type type) const;
	void createPickup(SceneNode& node, const TextureHolder& textures) const;
	float cropNumber(int number) const;
	// tank type, sprite
	Type mType;
	Rotation mRotation;
	sf::Sprite mSprite;
	bool mChangeHeavyTankSpriteTick;
	sf::Time mChangeHeavyTankSpriteTickTime;
	sf::Sprite mPoints;
	sf::Time mShowPoints;
	// bullets
	mutable int mBullets;
	bool mIsFiring;
	sf::Time mFireCountdown;
	Command mFireCommand;
	// bonuses
	bool mIsBonusTank;
	bool mBonusTankFrameTick;
	sf::Time mBonusTankFrameTime;
	Command mDropPickupCommand;
	bool mSpawnedPickup;
	// moving
	int mFrames;
	int mFramesPerMove;
	bool mTrackTick;
	bool mHasMoved;
	bool mCanMove;
	bool mIsInOccupiedSpawnpoint;
	// sliding in ice
	bool mIsForcedToMove;
	sf::Time mSlidingTime;
	// stopping tank
	bool mIsStopped;
	bool mShowStoppedTank;
	sf::Time mStopDuration;
	bool mShowTankTick;
	sf::Time mStoppedTankTick;
	// invincibility on
	bool mIsInvincible;
	sf::Time mInvincibilityTime;
	Animation mInvincibilityFrame;
	// spawning
	bool mIsSpawning;
	sf::Time mSpawningTime;
	Animation mSpawningFrame;
	// explosion
	Animation mSmallExplosion;
	Animation mBigExplosion;
	bool mShowExplosion;
	// to know everything
	CommandQueue mCommandQueue;
	Tilemap& mTilemap;
	PlayerData* mPlayerData;
	TankFunction mCommand;
	bool mHasDied;
};