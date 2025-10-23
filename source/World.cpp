#include "World.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Utility.hpp"
#include "TextNode.hpp"
#include "TileNode.hpp"
#include "RectangleNode.hpp"
#include "StatisticsNode.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>

World::Spawnpoint::Spawnpoint(Tank::Type type, bool isBonusTank) :
	type(type),
	isBonusTank(isBonusTank)
{

}

World::World(TextureHolder& textures, FontHolder& fonts, Players& players, bool isMultiplayer, int level) :
	mTextures(textures),
	mFonts(fonts),
	mSceneGraph(),
	mSceneLayers(),
	mTilemap(),
	mPlayers(players),
	mPlayersTanks(),
	mActiveEnemies(),
	mActiveTanks(),
	mEnemySpawnpoints(),
	mRespawnTankTime(),
	mLevelTime(),
	mShowGameOver(true),
	mShowGameOverMultiplayer(true),
	mIsMultiplayer(isMultiplayer),
	mRespawnTime(190 - (level - 1) * 4 - (isMultiplayer? 2 : 1 - 1) * 20),
	mEnemyTanks(20)
{
	buildScene(level);
}

void World::update(sf::Time deltaTime)
{
	commandEnemies(deltaTime);
	// clear command queue
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), deltaTime);
	handleCollisions();
	// erase dead tanks
	eraseDeadTanks(mActiveEnemies);
	eraseDeadTanks(mPlayersTanks);
	eraseDeadTanks(mActiveTanks);
	mSceneGraph.removeWrecks();
	// respawning
	size_t maxEnemies = mIsMultiplayer ? 6 : 4;
	if (mRespawnTankTime <= sf::Time::Zero && mActiveEnemies.size() < maxEnemies)
	{
		mRespawnTankTime = sf::seconds(mRespawnTime / 60.f);
		spawnEnemy(deltaTime);
	}
	else
		mRespawnTankTime -= deltaTime;
	respawnPlayers();
	// update
	mTilemap.update(deltaTime);
	mSceneGraph.update(deltaTime, mCommandQueue);
	checkGameOver();
}

void World::draw(sf::RenderWindow& target) const
{
	target.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

int World::numberOfAlivePlayers() const
{
	return std::count_if(mPlayers.begin(), mPlayers.end(), [](const Player player) {return player.getPlayerData().canSpawn();});
}

bool World::hasAllActiveEnemiesDied() const
{
	return mActiveEnemies.empty();
}

bool World::hasRemainingEnemies() const
{
	return !mEnemySpawnpoints.empty();
}

bool World::isBaseDestroyed() const
{
	return mTilemap.isBaseDestroyed();
}

static bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();
	if (type1 & category1 && type2 & category2)
		return true;
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
		return false;
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
	for (auto& tank : mActiveTanks)
	{
		if (tank->isSpawned() && !tank->isDestroyed())
			tank->checkTanksAhead(mActiveTanks);
	}
	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::EnemyTank, Category::PlayerProjectiles))
		{
			// destroying enemy tanks and getting points
			auto& tank = static_cast<Tank&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			if (tank.isSpawned())
			{
				tank.damage(projectile.getDamage(), projectile.getPlayerData());
				projectile.destroy();
				break;
			}
		}
		else if (matchesCategories(pair, Category::PlayerTanks, Category::EnemyProjectile))
		{
			// destroying player tanks
			auto& tank = static_cast<Tank&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			if (tank.isSpawned())
			{
				if (!tank.isInvincible())
					tank.damage(projectile.getDamage());
				projectile.destroy();
				break;
			}
		}
		else if (matchesCategories(pair, Category::FirstPlayerTank, Category::SecondPlayerProjectile) ||
			matchesCategories(pair, Category::SecondPlayerTank, Category::FirstPlayerProjectile))
		{
			// friendly fire
			auto& tank = static_cast<Tank&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);
			tank.stopTank(sf::seconds(3), false);
			projectile.destroy();
			break;
		}
		else if (matchesCategories(pair, Category::PlayerProjectiles, Category::EnemyProjectile) ||
			matchesCategories(pair, Category::PlayerProjectiles, Category::PlayerProjectiles))
		{
			// destroying bullets intersected with other bullets
			auto& firstProjectile = static_cast<Projectile&>(*pair.first);
			auto& secondProjectile = static_cast<Projectile&>(*pair.second);
			firstProjectile.remove();
			secondProjectile.remove();
			break;
		}
		else if (matchesCategories(pair, Category::PlayerTanks, Category::Pickup))
		{
			// applying bonuses
			auto& player = static_cast<Tank&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);
			pickup.apply(player);
		}
	}
}

void World::buildScene(int level)
{
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == Entities) ? Category::Scene : Category::None;
		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}
	mTilemap.load("Media/Maps/Map" + std::to_string(level) + ".txt");
	std::unique_ptr<RectangleNode> background(new RectangleNode(sf::FloatRect(16, 8, 208, 208), sf::Color::Black));
	mSceneLayers[Background]->attachChild(std::move(background));
	std::unique_ptr<TileNode> firstLayer(new TileNode(mTextures, mTilemap, true));
	firstLayer->setPosition(16, 8);
	firstLayer->addLayer(Tilemap::Air);
	firstLayer->addLayer(Tilemap::Brick);
	firstLayer->addLayer(Tilemap::Concrete);
	firstLayer->addLayer(Tilemap::Ice);
	firstLayer->addLayer(Tilemap::Water);
	firstLayer->addLayer(Tilemap::FirstHalfOfBase);
	firstLayer->addLayer(Tilemap::SecondHalfOfBase);
	firstLayer->addLayer(Tilemap::DestroyedFirstHalfOfBase);
	firstLayer->addLayer(Tilemap::DestroyedSecondHalfOfBase);
	firstLayer->construct();
	mSceneLayers[FirstLayer]->attachChild(std::move(firstLayer));
	std::unique_ptr<TileNode> secondLayer(new TileNode(mTextures, mTilemap, false));
	secondLayer->setPosition(16, 8);
	secondLayer->addLayer(Tilemap::Leaf);
	secondLayer->construct();
	mSceneLayers[SecondLayer]->attachChild(std::move(secondLayer));
	for (int i = 0; i < mEnemyTanks; ++i)
		addEnemy();
	std::unique_ptr<StatisticsNode> statistics(new StatisticsNode(mTextures, mFonts, mPlayers, mEnemySpawnpoints, mIsMultiplayer, level));
	statistics->setPosition(224, 0);
	statistics->construct();
	mSceneLayers[SecondLayer]->attachChild(std::move(statistics));
}

void World::checkGameOver()
{
	if ((numberOfAlivePlayers() == 0 || isBaseDestroyed()) && mShowGameOver)
	{
		std::unique_ptr<TextNode> gameOverText(new TextNode(mFonts, "GAME\nOVER"));
		gameOverText->setPosition(104, 224);
		gameOverText->setCommand([](TextNode* textNode) {
			if (textNode->getWorldPosition().y > 96)
				textNode->move(0, -1);
			});
		gameOverText.get()->getText().setFillColor(sf::Color(224, 80, 0));
		mSceneLayers[SecondLayer]->attachChild(std::move(gameOverText));
		mShowGameOver = false;
	}
	if (numberOfAlivePlayers() == 1 && mIsMultiplayer && mShowGameOverMultiplayer)
	{
		std::unique_ptr<TextNode> gameOverText(new TextNode(mFonts, "GAME\nOVER"));
		gameOverText.get()->getText().setFillColor(sf::Color(224, 80, 0));
		if (!mPlayers[0].getPlayerData().canSpawn())
		{
			gameOverText->setPosition(16, 200);
			gameOverText->setCommand([](TextNode* textNode) {
				if (textNode->getWorldPosition().x < 72)
					textNode->move(1, 0);
				});
		}
		else
		{
			gameOverText->setPosition(192, 200);
			gameOverText->setCommand([](TextNode* textNode) {
				if (textNode->getWorldPosition().x > 137)
					textNode->move(-1, 0);
				});
		}
		mSceneLayers[SecondLayer]->attachChild(std::move(gameOverText));
		mShowGameOverMultiplayer = false;
	}
}

void World::respawnPlayers()
{
	for (auto& player : mPlayers)
	{
		if (!player.getPlayerData().isAlive() && player.getPlayerData().canSpawn())
			spawnPlayer(player.getPlayerData().setAlive(true));
	}
}

void World::spawnPlayer(PlayerData& playerData)
{
	std::unique_ptr<Tank> player(new Tank(playerData.getType(), mTextures, mTilemap, playerData));
	player->setPosition(playerData.getSpawnpoint());
	mPlayersTanks.push_back(player.get());
	mActiveTanks.push_back(player.get());
	mSceneLayers[Entities]->attachChild(std::move(player));
}

void World::addEnemy()
{
	Tank::Type type = randomInt(8) == 0 ? Tank::EnemyHeavyTank : static_cast<Tank::Type>(Tank::EnemyRegularTank + randomInt(3));
	bool isBonusTank = randomInt(4) == 0;
	Spawnpoint spawn(type, isBonusTank);
	mEnemySpawnpoints.push_back(spawn);
}

void World::spawnEnemy(sf::Time deltaTime)
{
	if (!mEnemySpawnpoints.empty())
	{
		Spawnpoint spawn = mEnemySpawnpoints.back();
		mEnemySpawnpoints.pop_back();
		std::unique_ptr<Tank> enemy(new Tank(spawn.type, mTextures, mTilemap, spawn.isBonusTank));
		enemy->setPosition(enemySpawns[randomInt(3)]);
		mActiveEnemies.push_back(enemy.get());
		mActiveTanks.push_back(enemy.get());
		mSceneLayers[Entities]->attachChild(std::move(enemy));
	}
}

void World::commandEnemies(sf::Time deltaTime)
{
	mLevelTime += deltaTime;
	float periodDuration = mRespawnTime / 8.f;
	Command command;
	command.category = Category::EnemyTank;
	if (mLevelTime < sf::seconds(periodDuration))
	{
		// send command to behave randomly
		command.action = derivedAction<Tank>([](Tank& tank, sf::Time) {
			auto randomRotation = [](Tank* tank) {
				Entity::Rotation rotation = static_cast<Entity::Rotation>(randomInt(Entity::RotationCount));
				tank->setRotation(rotation);
				};
			tank.setCommand(randomRotation); });
	}
	else if (mLevelTime < sf::seconds(periodDuration * 2))
	{
		// send command to go after players
		command.action = derivedAction<Tank>([this](Tank& tank, sf::Time) {
			auto rotateAfterPlayers = [this](Tank* tank) {
				Entity::Rotation rotation = Entity::Down;
				if (mPlayersTanks.size() == 1)
					rotation = getRotationToTank(mPlayersTanks[0], tank);
				else if (mPlayersTanks.size() == 2)
				{
					size_t counter = 1;
					for (auto& enemyTank : mActiveEnemies)
					{
						if (counter < mActiveEnemies.size())
							rotation = getRotationToTank(mPlayersTanks[0], tank);
						else
							rotation = getRotationToTank(mPlayersTanks[1], tank);
						counter++;
					}
				}
				tank->setRotation(rotation);
				};
			tank.setCommand(rotateAfterPlayers); });
	}
	else
	{
		// send command to go after base
		command.action = derivedAction<Tank>([this](Tank& tank, sf::Time) {
			auto rotateAfterBase = [this](Tank* tank) {
				Entity::Rotation rotation = getRotationToPosition(sf::Vector2f(112.f, 200.f), tank);
				tank->setRotation(rotation);
				};
			tank.setCommand(rotateAfterBase); });
	}
	getCommandQueue().push(command);
}

void World::eraseDeadTanks(Tanks& tanks)
{
	auto deadTanksBegin = std::remove_if(tanks.begin(), tanks.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	tanks.erase(deadTanksBegin, tanks.end());
}

Entity::Rotation World::getRotationToTank(Tank* rhs, Tank* lhs) const
{
	sf::Vector2f directionToGo = distance(*rhs, *lhs);
	directionToGo.y = -directionToGo.y;
	// move right up
	if (directionToGo.x >= 0 && directionToGo.y >= 0)
		return randomInt(2) ? Entity::Right : Entity::Up;
	// move right down
	else if (directionToGo.x >= 0 && directionToGo.y < 0)
		return randomInt(2) ? Entity::Right : Entity::Down;
	// move left down
	else if (directionToGo.x < 0 && directionToGo.y < 0)
		return randomInt(2) ? Entity::Left : Entity::Down;
	// move left up
	else
		return randomInt(2) ? Entity::Left : Entity::Up;
}

Entity::Rotation World::getRotationToPosition(sf::Vector2f position, Tank* tank) const
{
	sf::Vector2f directionToGo = position - tank->getWorldPosition();
	directionToGo.y = -directionToGo.y;
	// move right up
	if (directionToGo.x >= 0 && directionToGo.y >= 0)
		return randomInt(2) ? Entity::Right : Entity::Up;
	// move right down
	else if (directionToGo.x >= 0 && directionToGo.y < 0)
		return randomInt(2) ? Entity::Right : Entity::Down;
	// move left down
	else if (directionToGo.x < 0 && directionToGo.y < 0)
		return randomInt(2) ? Entity::Left : Entity::Down;
	// move left up
	else
		return randomInt(2) ? Entity::Left : Entity::Up;
}