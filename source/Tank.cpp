#include "Tank.hpp"
#include "TileNode.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "PlayerData.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace
{
	const std::vector<TankData> Table = initializeTankData();
}

Tank::Tank(Type type, const TextureHolder& textures, Tilemap& tilemap, PlayerData& playerData) :
	Entity(Table[type].hitpoints),
	// tank type, sprite
	mType(type),
	mRotation(Rotation::Up),
	mSprite(textures.get(Table[type].texture), sf::IntRect(mRotation * 32, playerData.getGrade() * 16, 16, 16)),
	mChangeHeavyTankSpriteTick(false),
	mChangeHeavyTankSpriteTickTime(),
	// bullets
	mBullets(playerData.getGrade() < 2? 1 : 2),
	mIsFiring(false),
	// bonuses
	mIsBonusTank(false),
	mBonusTankFrameTick(false),
	mBonusTankFrameTime(),
	mSpawnedPickup(false),
	// moving
	mFrames(0),
	mFramesPerMove(Table[type].framesPerMove),
	mTrackTick(false),
	mHasMoved(false),
	mCanMove(true),
	mIsInOccupiedSpawnpoint(true),
	// sliding in ice
	mIsForcedToMove(false),
	mSlidingTime(),
	// stopping tank
	mIsStopped(false),
	mShowStoppedTank(false),
	mStopDuration(),
	mShowTankTick(true),
	mStoppedTankTick(),
	// invincibility on
	mIsInvincible(true),
	mInvincibilityTime(sf::seconds(2.f)),
	mInvincibilityFrame(textures.get(Textures::InvicibilityFrames), sf::Vector2i(16, 16), 2, sf::seconds(0.12f)),
	// spawning
	mIsSpawning(true),
	mSpawningTime(sf::seconds(1.f)),
	mSpawningFrame(textures.get(Textures::SpawningFrames), sf::Vector2i(16, 16), 6, sf::seconds(0.33f)),
	// explosion
	mSmallExplosion(textures.get(Textures::SmallExplosionFrames), sf::Vector2i(16, 16), 3, sf::seconds(18.f / 60.f)),
	mBigExplosion(textures.get(Textures::BigExplosionFrames), sf::Vector2i(32, 32), 2, sf::seconds(12.f / 60.f)),
	mShowExplosion(true),
	// to know everything
	mCommandQueue(),
	mTilemap(tilemap),
	mPlayerData(&playerData),
	mHasDied(false)
{
	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures, &playerData](SceneNode& node, sf::Time deltaTime)
		{
			mBullets -= 1;
			Projectile::Type type = getCategory() == Category::FirstPlayerTank ?
				(playerData.getGrade() < 1 ? Projectile::FirstPlayerSlowBullet : Projectile::FirstPlayerFastBullet) :
				(playerData.getGrade() < 1 ? Projectile::SecondPlayerSlowBullet : Projectile::SecondPlayerFastBullet);
			createProjectile(node, textures, type);
		};
	mDropPickupCommand.category = Category::Scene;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time deltaTime)
		{
			createPickup(node, textures);
		};
	mInvincibilityFrame.setRepeating(true);
	mSpawningFrame.setRepeating(true);
	mBigExplosion.move(-8, -8);
}

Tank::Tank(Type type, const TextureHolder& textures, Tilemap& tilemap, bool isBonusTank) :
	Entity(Table[type].hitpoints),
	mType(type),
	mRotation(Rotation::Down),
	mSprite(textures.get(Table[type].texture), sf::IntRect(mRotation * 32, 0 * 16, 16, 16)),
	mChangeHeavyTankSpriteTick(false),
	mChangeHeavyTankSpriteTickTime(),
	mPoints(textures.get(Textures::Points), sf::IntRect((type - EnemyRegularTank) * 16, 0, 16, 16)),
	mShowPoints(sf::seconds(0.5f)),
	// bullets
	mBullets(1),
	mIsFiring(false),
	// bonuses
	mIsBonusTank(isBonusTank),
	mBonusTankFrameTick(false),
	mBonusTankFrameTime(),
	mSpawnedPickup(false),
	// moving
	mFrames(0),
	mFramesPerMove(Table[type].framesPerMove),
	mTrackTick(false),
	mHasMoved(false),
	mCanMove(true),
	mIsInOccupiedSpawnpoint(true),
	// sliding in ice
	mIsForcedToMove(false),
	mSlidingTime(),
	// stopping tank
	mIsStopped(false),
	mShowStoppedTank(false),
	mStopDuration(),
	mShowTankTick(true),
	mStoppedTankTick(),
	// invincibility on
	mIsInvincible(false),
	// spawning
	mIsSpawning(true),
	mSpawningTime(sf::seconds(1.f)),
	mSpawningFrame(textures.get(Textures::SpawningFrames), sf::Vector2i(16, 16), 6, sf::seconds(0.33f)),
	// explosion
	mSmallExplosion(textures.get(Textures::SmallExplosionFrames), sf::Vector2i(16, 16), 3, sf::seconds(18.f / 60.f)),
	mBigExplosion(textures.get(Textures::BigExplosionFrames), sf::Vector2i(32, 32), 2, sf::seconds(12.f / 60.f)),
	mShowExplosion(true),
	// to know everything
	mCommandQueue(),
	mTilemap(tilemap),
	mPlayerData(nullptr),
	mHasDied(false)
{
	mFireCommand.category = Category::Scene;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time deltaTime)
		{
			mBullets -= 1;
			Projectile::Type type = mType == EnemyRapidFireTank ? Projectile::EnemyFastBullet : Projectile::EnemySlowBullet;
			createProjectile(node, textures, type);
		};
	mDropPickupCommand.category = Category::Scene;
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time deltaTime)
		{
			createPickup(node, textures);
		};
	mInvincibilityFrame.setRepeating(true);
	mSpawningFrame.setRepeating(true);
	mBigExplosion.move(-8, -8);
}

unsigned int Tank::getCategory() const
{
	switch (mType)
	{
	case FirstPlayer:
		return Category::FirstPlayerTank;
	case SecondPlayer:
		return Category::SecondPlayerTank;
	default:
		return Category::EnemyTank;
	}
}

sf::FloatRect Tank::getBoundingRect() const
{
	sf::FloatRect boundingRect = mSprite.getGlobalBounds();
	boundingRect.left = cropNumber(static_cast<int>(boundingRect.left));
	boundingRect.top = cropNumber(static_cast<int>(boundingRect.top));
	return getWorldTransform().transformRect(boundingRect);
}

bool Tank::isMarkedForRemoval() const
{
	return isDestroyed() && (isSequenceEnded() || !mShowExplosion);
}

void Tank::remove()
{
	Entity::remove();
	mShowExplosion = false;
}

void Tank::destroy()
{
	if (isSpawned())
	{
		mPlayerData = nullptr;
		Entity::destroy();
	}
}

void Tank::damage(int points)
{
	Entity::damage(points);
	if (mType == EnemyHeavyTank)
		checkPickupDrop(mCommandQueue);
}

void Tank::damage(int points, PlayerData* player)
{
	damage(points);
	mPlayerData = player;
}

bool Tank::isPlayer() const
{
	return mType == FirstPlayer || mType == SecondPlayer;
}

bool Tank::isInvincible() const
{
	return mIsInvincible;
}

bool Tank::isSpawned() const
{
	return !mIsSpawning;
}

void Tank::moveTank(Rotation rotation)
{
	if (!mIsStopped)
	{
		mRotation = rotation;
		mHasMoved = true;
	}
}

void Tank::stopTank(sf::Time duration, bool showTank)
{
	if (!mIsStopped)
	{
		mIsStopped = true;
		mShowStoppedTank = showTank;
		mStopDuration = duration;
		mStoppedTankTick = sf::seconds(8.f / 60.f);
	}
}

void Tank::fire()
{
	if (!mIsStopped && isSpawned())
		mIsFiring = true;
}

void Tank::makeInvincible(sf::Time duration)
{
	mIsInvincible = true;
	mInvincibilityTime = duration;
}

void Tank::upgrade()
{
	mPlayerData->getGrade() + 1 > 3 ? addLife() : addGrade();
	if (mPlayerData->getGrade() == 2)
		mBullets += 1;
}

void Tank::addLife()
{
	mPlayerData->modifyLifes(1);
}

void Tank::addGrade()
{
	mPlayerData->modifyGrade(1);
}

void Tank::setPendingCommand(const Command& command)
{
	mCommandQueue.push(command);
}

void Tank::defendBase() const
{
	mTilemap.defendBase();
}

void Tank::checkTanksAhead(const Tanks& tanks)
{
	bool intersected = false;
	for (auto& tank : tanks)
	{
		if (tank != this && tank->isSpawned() && !tank->isDestroyed())
		{
			sf::Vector2f velocity;
			if (mRotation == Rotation::Left or mRotation == Rotation::Right)
				velocity.x += mRotation == Rotation::Left ? -8 : 8;
			else if (mRotation == Rotation::Up or mRotation == Rotation::Down)
				velocity.y += mRotation == Rotation::Up ? -8 : 8;
			sf::FloatRect thisTankRect = this->getBoundingRect();
			sf::FloatRect otherTankRect = tank->getBoundingRect();
			thisTankRect.left += velocity.x;
			thisTankRect.top += velocity.y;
			if (thisTankRect.intersects(otherTankRect))
			{
				sf::FloatRect nextPosition = getBoundingRect();
				nextPosition.left += velocity.x / 8;
				nextPosition.top += velocity.y / 8;
				if (nextPosition.intersects(otherTankRect))
					intersected = true;
			}
		}
	}
	if (intersected)
	{
		if (mIsInOccupiedSpawnpoint)
			intersected = false;
	}
	else
		mIsInOccupiedSpawnpoint = false;
	mCanMove = !intersected;
}

void Tank::setRotation(Rotation rotation)
{
	mRotation = rotation;
}

void Tank::setCommand(TankFunction function)
{
	mCommand = function;
}

PlayerData& Tank::getPlayerData()
{
	return *mPlayerData;
}

void Tank::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!isSpawned())
		target.draw(mSpawningFrame, states);
	else if (mShowTankTick && !isDestroyed())
	{
		target.draw(mSprite, states);
		if (isInvincible())
			target.draw(mInvincibilityFrame, states);
	}
	else if (mShowExplosion && isDestroyed())
	{
		if (!mSmallExplosion.isFinished())
			target.draw(mSmallExplosion, states);
		else if (!mBigExplosion.isFinished())
			target.draw(mBigExplosion, states);
		else if (mPlayerData != nullptr)
			target.draw(mPoints, states);
	}
}

void Tank::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	if (isDestroyed())
	{
		checkPickupDrop(commands);
		updateDestroyedTank(deltaTime);
	}
	else if (isSpawned())
	{
		if (!mCommandQueue.isEmpty())
			commands.push(mCommandQueue.pop());
		checkProjectileLaunch(deltaTime, commands);
		updateTimers(deltaTime);
		updateMovementPattern();
		updatePosition();
		updateTankSprite();
	}
	else
		updateSpawning(deltaTime);
}

void Tank::updateDestroyedTank(sf::Time deltaTime)
{
	if (!mHasDied)
	{
		if (isPlayer())
			mPlayerData->destroyed();
		else if (mPlayerData != nullptr)
			mPlayerData->recordEnemyTank(mType);
		mHasDied = true;
	}
	if (!updateExplosion(deltaTime))
		mShowPoints -= deltaTime;
}

void Tank::updateSpawning(sf::Time deltaTime)
{
	if (mSpawningTime <= sf::Time::Zero)
		mIsSpawning = false;
	else
	{
		mSpawningTime -= deltaTime;
		mSpawningFrame.update(deltaTime);
	}
}

void Tank::updateTimers(sf::Time deltaTime)
{
	checkBonusTankSprite(deltaTime);
	checkHeavyTankSprite(deltaTime);
	checkTankSliding(deltaTime);
	checkTankStopped(deltaTime);
	checkTankInvincibility(deltaTime);
}

void Tank::checkBonusTankSprite(sf::Time deltaTime)
{
	if (mIsBonusTank)
	{
		if (mBonusTankFrameTime <= sf::Time::Zero)
		{
			mBonusTankFrameTick = !mBonusTankFrameTick;
			mBonusTankFrameTime = sf::seconds(8.f / 60.f);
		}
		else
			mBonusTankFrameTime -= deltaTime;
	}
}

void Tank::checkHeavyTankSprite(sf::Time deltaTime)
{
	if (mChangeHeavyTankSpriteTickTime <= sf::Time::Zero && mType == EnemyHeavyTank)
	{
		mChangeHeavyTankSpriteTickTime = sf::seconds(4.f / 60.f);
		mChangeHeavyTankSpriteTick = !mChangeHeavyTankSpriteTick;
	}
	else
		mChangeHeavyTankSpriteTickTime -= deltaTime;
}

void Tank::checkTankSliding(sf::Time deltaTime)
{
	if (mSlidingTime <= sf::Time::Zero)
		mIsForcedToMove = false;
	else
		mSlidingTime -= deltaTime;
}

void Tank::checkTankStopped(sf::Time deltaTime)
{
	if (mStopDuration <= sf::Time::Zero)
	{
		mIsStopped = false;
		mShowTankTick = true;
	}
	else
	{
		if (!mShowStoppedTank)
		{
			if (mStoppedTankTick <= sf::Time::Zero)
			{
				mShowTankTick = !mShowTankTick;
				mStoppedTankTick = sf::seconds(8.f / 60.f);
			}
			else
				mStoppedTankTick -= deltaTime;
		}
		mStopDuration -= deltaTime;
	}
}

void Tank::checkTankInvincibility(sf::Time deltaTime)
{
	if (mInvincibilityTime <= sf::Time::Zero && isSpawned())
		mIsInvincible = false;
	else
	{
		mInvincibilityTime -= deltaTime;
		mInvincibilityFrame.update(deltaTime);
	}
}

void Tank::updateTankSprite()
{
	sf::IntRect textureRect(mRotation * 32, (isPlayer() ? mPlayerData->getGrade() : 0) * 16, 16, 16);
	if (mType == EnemyHeavyTank)
	{
		int health = getHitpoints();
		if (health == 4)
		{
			if (mChangeHeavyTankSpriteTick)
				textureRect.top = 2 * 16; // green - yellow
			else
				textureRect.top = 1 * 16;
		}
		else if (health == 3)
		{
			if (mChangeHeavyTankSpriteTick)
				textureRect.top = 2 * 16; // yellow - white
		}
		else if (health == 2)
		{
			if (mChangeHeavyTankSpriteTick)
				textureRect.top = 1 * 16; // green - white
		}
		if (mBonusTankFrameTick)
			textureRect.top = 3 * 16;
		// make his color change as changes his health
		textureRect.left += mTrackTick ? 16 : 0;
	}
	else
	{
		textureRect.left += mTrackTick? 16 : 0;
		textureRect.top += mBonusTankFrameTick? 16 : 0;
	}
	mSprite.setTextureRect(textureRect);
}

void Tank::updateMovementPattern()
{
	if (!isPlayer() && !mIsStopped)
	{
		auto tankPosition = getWorldPosition();
		// change tank rotation if reached another tile
		if (static_cast<int>(tankPosition.x) % 8 == 0 && static_cast<int>(tankPosition.y) % 8 == 0 && randomInt(16) == 0)
			mCommand(this);
		else if (!mCanMove && randomInt(4) == 0)
		{
			// move clock or anticlockwise
			if (mRotation == Rotation::Left || mRotation == Rotation::Right)
				mRotation = randomInt(2) ? Rotation::Up : Rotation::Down;
			else if (mRotation == Rotation::Up || mRotation == Rotation::Down)
				mRotation = randomInt(2) ? Rotation::Left : Rotation::Right;
		}
		moveTank(mRotation);
	}
}

bool Tank::isSequenceEnded() const
{
	return mShowPoints <= sf::Time::Zero && mBigExplosion.isFinished();
}

bool Tank::updateExplosion(sf::Time deltaTime)
{
	if (!mSmallExplosion.isFinished())
		mSmallExplosion.update(deltaTime);
	else if (!mBigExplosion.isFinished())
		mBigExplosion.update(deltaTime);
	else
		return false;
	return true;
}

void Tank::updatePosition()
{
	sf::Vector2f velocity;
	sf::Vector2f spritePosition = getWorldPosition();
	if (mRotation == Rotation::Left || mRotation == Rotation::Right)
	{
		velocity.x += mRotation == Rotation::Left ? -1 : 1;
		setPosition(spritePosition.x, cropNumber(static_cast<int>(spritePosition.y)));
	}
	else if (mRotation == Rotation::Up || mRotation == Rotation::Down)
	{
		velocity.y += mRotation == Rotation::Up ? -1 : 1;
		setPosition(cropNumber(static_cast<int>(spritePosition.x)), spritePosition.y);
	}
	if (mHasMoved || mIsForcedToMove)
		mFrames += 1;
	else
		mFrames = 0;
	if (mFrames % mFramesPerMove != 0 || (mFramesPerMove == 1) && mHasMoved)
	{
		if (checkNextPosition(velocity))
			move(velocity);
		if (!mIsForcedToMove || mHasMoved)
			mTrackTick = !mTrackTick;
	}
	mHasMoved = false;
}

bool Tank::checkNextPosition(sf::Vector2f velocity)
{
	sf::FloatRect nextPosition = getBoundingRect();
	nextPosition.left += velocity.x;
	nextPosition.top += velocity.y;
	bool onIce = false;
	if (!mCanMove)
		return false;
	for (const auto& tile : mTilemap.get())
	{
		Tilemap::Type tileNumber = tile.tileNumber;
		if (tileNumber == Tilemap::Air || tileNumber == Tilemap::Leaf)
			continue;
		sf::FloatRect tileRect(static_cast<float>(16 + tile.x * 8), static_cast<float>(8 + tile.y * 8), 8, 8);
		if (tileRect.intersects(nextPosition))
		{
			if (tileNumber == Tilemap::Ice && isPlayer())
			{
				onIce = true;
				mIsForcedToMove = true;
				if (mHasMoved)
					mSlidingTime = sf::seconds(28.f / 60.f);
			}
			else
			{
				mCanMove = false;
				return false;
			}
		}
	}
	if (!onIce)
		mSlidingTime = sf::Time::Zero;
	if (16 <= nextPosition.left && nextPosition.left <= 208 && 8 <= nextPosition.top && nextPosition.top <= 200)
		return true;
	return false;
}

void Tank::checkPickupDrop(CommandQueue& commands)
{
	if (mIsBonusTank && !mSpawnedPickup)
	{
		commands.push(mDropPickupCommand);
		mSpawnedPickup = true;
		mIsBonusTank = false;
		mBonusTankFrameTick = false;
	}
}

void Tank::checkProjectileLaunch(sf::Time deltaTime, CommandQueue& commands)
{
	if (!isPlayer() && randomInt(32) == 0)
		fire();
	if (mIsFiring && mFireCountdown <= sf::Time::Zero && mBullets > 0)
	{
		commands.push(mFireCommand);
		mFireCountdown = Table[mType].fireInterval;
		mIsFiring = false;
	}
	else
	{
		mFireCountdown -= deltaTime;
		mIsFiring = false;
	}
}

void Tank::createProjectile(SceneNode& node, const TextureHolder& textures, Projectile::Type type) const
{
	std::unique_ptr<Projectile> projectile(isPlayer() ?
		new Projectile(type, textures, mTilemap, mRotation, mPlayerData, mBullets) :
		new Projectile(type, textures, mTilemap, mRotation, mBullets));
	sf::Vector2f offset;
	if (mRotation == Rotation::Left || mRotation == Rotation::Right)
	{
		offset.x += mRotation == Rotation::Left ? 0 : 12;
		offset.y += 6;
	}
	else if (mRotation == Rotation::Up || mRotation == Rotation::Down)
	{
		offset.x += 6;
		offset.y += mRotation == Rotation::Up ? 0 : 12;
	}
	projectile->move(getWorldPosition() + offset);
	node.attachChild(std::move(projectile));
}

void Tank::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	Pickup::Type type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));
	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	std::vector<Tilemap::Tile> tiles;
	for (const auto& tile : mTilemap.get())
	{
		if (tile.tileNumber == Tilemap::Air && tile.x < 25 && tile.y < 25)
			tiles.push_back(tile);
	}
	Tilemap::Tile randomTile = tiles[randomInt(tiles.size() - 1)];
	sf::Vector2f randomPosition(static_cast<float>(16 + randomTile.x * 8), static_cast<float>(8 + randomTile.y * 8));
	pickup->setPosition(randomPosition);
	node.attachChild(std::move(pickup));
}

float Tank::cropNumber(int number) const
{
	if (0 <= number % 16 && number % 16 <= 4)
		return static_cast<float>(number - number % 16);
	else if (4 < number % 16 && number % 16 <= 12)
		return static_cast<float>(number - (number % 16) + 8);
	else
		return static_cast<float>(number - (number % 16) + 16);
}