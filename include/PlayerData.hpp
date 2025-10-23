#pragma once

#include "Tank.hpp"
#include <SFML/System/Vector2.hpp>
#include <map>

class PlayerData
{
public:
	using Statistics = std::map<Tank::Type, int>;
	PlayerData(Tank::Type type, sf::Vector2f spawnpoint);
	Tank::Type getType() const;
	sf::Vector2f getSpawnpoint() const;
	PlayerData& modifyLifes(int lifes);
	PlayerData& modifyGrade(int grade);
	PlayerData& modifyPoints(int points);
	PlayerData& setAlive(bool flag);
	int getLifes() const;
	int getGrade() const;
	int getPoints() const;
	bool isAlive() const;
	bool canSpawn() const;
	void clear();
	void reset();
	void destroyed();
	void recordEnemyTank(Tank::Type enemyType);
	Statistics resetStatistics();
private:
	Tank::Type mType;
	sf::Vector2f mSpawnpoint;
	Statistics mDestroyedEnemyTanks;
	int mLifes;
	int mGrade;
	int mPoints;
	bool mIsAlive;
};