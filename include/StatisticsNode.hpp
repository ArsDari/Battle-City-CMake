#pragma once

#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "World.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

class StatisticsNode : public SceneNode
{
public:
	explicit StatisticsNode(const TextureHolder& textures, const FontHolder& fonts, const World::Players& players, const std::vector<World::Spawnpoint>& enemies, bool isMultiplayer, int level);
	void construct();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	std::string setLevelString(int level) const;
	std::string getLifesString(const PlayerData& playerData) const;
	sf::Texture mTileset;
	sf::Vector2f mTileSize;
	sf::VertexArray mVertices;
	sf::Sprite mStatistics;
	const World::Players& mPlayers;
	const std::vector<World::Spawnpoint>& mEnemies;
	int mNumOfTotalEnemies;
	bool mIsMultiplayer;
	sf::Text mFirstPlayerText;
	sf::Text mSecondPlayerText;
	sf::Text mLevel;
};