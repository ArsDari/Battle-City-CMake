#pragma once

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "Tilemap.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"
#include "Player.hpp"
#include <SFML/System/NonCopyable.hpp>
#include <array>
#include <queue>

namespace sf
{
	class RenderWindow;
}

namespace
{
	sf::Vector2f enemySpawns[3]{ {16, 8}, {112, 8}, {208, 8} };
}

class World : private sf::NonCopyable
{
public:
	struct Spawnpoint
	{
		Spawnpoint(Tank::Type type, bool isBonusTank);
		Tank::Type type;
		bool isBonusTank;
	};
	using Players = std::vector<Player>;
	using Tanks = std::vector<Tank*>;
	explicit World(TextureHolder& textures, FontHolder& fonts, Players& players, bool isMultiplayer, int level);
	void update(sf::Time deltaTime);
	void draw(sf::RenderWindow& target) const;
	CommandQueue& getCommandQueue();
	int numberOfAlivePlayers() const;
	bool hasAllActiveEnemiesDied() const;
	bool hasRemainingEnemies() const;
	bool isBaseDestroyed() const;
private:
	void handleCollisions();
	void buildScene(int level);
	void checkGameOver();
	void respawnPlayers();
	void addEnemy();
	void spawnPlayer(PlayerData& player);
	void spawnEnemy(sf::Time deltaTime);
	void commandEnemies(sf::Time deltaTime);
	void eraseDeadTanks(Tanks& tanks);
	Entity::Rotation getRotationToTank(Tank* rhs, Tank* lhs) const;
	Entity::Rotation getRotationToPosition(sf::Vector2f position, Tank* tank) const;
	enum Layer
	{
		Background,
		FirstLayer,
		Entities,
		SecondLayer,
		LayerCount
	};
	TextureHolder& mTextures;
	FontHolder& mFonts;
	Players& mPlayers;
	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	CommandQueue mCommandQueue;
	Tilemap mTilemap;
	Tanks mPlayersTanks;
	Tanks mActiveEnemies;
	Tanks mActiveTanks;
	std::vector<Spawnpoint> mEnemySpawnpoints;
	sf::Time mRespawnTankTime;
	sf::Time mLevelTime;
	bool mShowGameOver;
	bool mShowGameOverMultiplayer;
	bool mIsMultiplayer;
	int mRespawnTime;
	int mEnemyTanks;
};