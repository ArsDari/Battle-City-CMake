#include "PlayerData.hpp"
#include <iostream>

PlayerData::PlayerData(Tank::Type type, sf::Vector2f spawnpoint) :
	mSpawnpoint(spawnpoint),
	mType(type),
	mLifes(-1),
	mGrade(0),
	mPoints(0),
	mIsAlive(false)
{
	mDestroyedEnemyTanks[Tank::EnemyRegularTank] = 0;
	mDestroyedEnemyTanks[Tank::EnemyAPC] = 0;
	mDestroyedEnemyTanks[Tank::EnemyRapidFireTank] = 0;
	mDestroyedEnemyTanks[Tank::EnemyHeavyTank] = 0;
}

Tank::Type PlayerData::getType() const
{
	return mType;
}

sf::Vector2f PlayerData::getSpawnpoint() const
{
	return mSpawnpoint;
}

PlayerData& PlayerData::modifyLifes(int lifes)
{
	mLifes += lifes;
	return *this;
}

PlayerData& PlayerData::modifyGrade(int grade)
{
	mGrade += grade;
	return *this;
}

PlayerData& PlayerData::modifyPoints(int points)
{
	mPoints += points;
	if (mPoints > 9999)
		mPoints %= 10000;
	return *this;
}

PlayerData& PlayerData::setAlive(bool flag)
{
	mIsAlive = flag;
	return *this;
}

int PlayerData::getLifes() const
{
	return mLifes;
}

int PlayerData::getGrade() const
{
	return mGrade;
}

int PlayerData::getPoints() const
{
	return mPoints;
}

bool PlayerData::isAlive() const
{
	return mIsAlive;
}

bool PlayerData::canSpawn() const
{
	return mLifes >= 0;
}

void PlayerData::clear()
{
	mLifes = -1;
	mGrade = 0;
	mPoints = 0;
}

void PlayerData::reset()
{
	mLifes = 2;
	mGrade = 0;
	mPoints = 0;
}

void PlayerData::destroyed()
{
	mLifes = mLifes - 1;
	mGrade = 0;
	mIsAlive = false;
}

void PlayerData::recordEnemyTank(Tank::Type enemyType)
{
	++mDestroyedEnemyTanks[enemyType];
}

PlayerData::Statistics PlayerData::resetStatistics()
{
	mIsAlive = false;
	Statistics returnStatistics(mDestroyedEnemyTanks);
	for (auto& pair : mDestroyedEnemyTanks)
		pair.second = 0;
	return returnStatistics;
}