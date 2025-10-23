#include "StatisticsNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

static int countDigit(int number)
{
	if (number < 1)
		return 1;
	return int(log10(number) + 1);
}

StatisticsNode::StatisticsNode(const TextureHolder& textures, const FontHolder& fonts, const World::Players& players, const std::vector<World::Spawnpoint>& enemies, bool isMultiplayer, int level) :
	mTileset(textures.get(Textures::TankMark)),
	mTileSize(mTileset.getSize()),
	mVertices(sf::Quads, enemies.size() * 4),
	mStatistics(textures.get(Textures::Statistics)),
	mPlayers(players),
	mEnemies(enemies),
	mNumOfTotalEnemies(enemies.size()),
	mIsMultiplayer(isMultiplayer),
	mFirstPlayerText(getLifesString(mPlayers[0].getPlayerData()), fonts.get(Fonts::Main), 8U),
	mSecondPlayerText(getLifesString(mPlayers[1].getPlayerData()), fonts.get(Fonts::Main), 8U),
	mLevel(setLevelString(level), fonts.get(Fonts::Main), 8U)
{
	mFirstPlayerText.setPosition(17, 136);
	mFirstPlayerText.setFillColor(sf::Color::Black);
	mSecondPlayerText.setPosition(17, 160);
	mSecondPlayerText.setFillColor(sf::Color::Black);
	mLevel.setPosition(8, 192);
	mLevel.setFillColor(sf::Color::Black);
}

void StatisticsNode::construct()
{
	int row = 0;
	for (int i = 0; i < mNumOfTotalEnemies; ++i)
	{
		int x = i % 2;
		sf::Vertex* quad = &mVertices[i * 4];
		quad[0].position = sf::Vector2f(static_cast<float>(8 + 8 * x), static_cast<float>(16 + 8 * row));
		quad[1].position = sf::Vector2f(static_cast<float>(8 + 8 * x + mTileSize.x), static_cast<float>(16 + 8 * row));
		quad[2].position = sf::Vector2f(static_cast<float>(8 + 8 * x + mTileSize.x), static_cast<float>(16 + 8 * row + mTileSize.y));
		quad[3].position = sf::Vector2f(static_cast<float>(8 + 8 * x), static_cast<float>(16 + 8 * row + mTileSize.y));
		if (static_cast<size_t>(i) < mEnemies.size())
		{
			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(mTileSize.x, 0);
			quad[2].texCoords = sf::Vector2f(mTileSize.x, mTileSize.y);
			quad[3].texCoords = sf::Vector2f(0, mTileSize.y);
		}
		else
		{
			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(0, 0);
			quad[2].texCoords = sf::Vector2f(0, 0);
			quad[3].texCoords = sf::Vector2f(0, 0);
		}
		if (i % 2 == 1)
			row += 1;
	}
}

void StatisticsNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mStatistics, states);
	target.draw(mFirstPlayerText, states);
	target.draw(mSecondPlayerText, states);
	target.draw(mLevel, states);
	if (!mIsMultiplayer)
	{
		sf::RectangleShape hideSecondPlayer(sf::Vector2f(16, 16));
		hideSecondPlayer.setFillColor(sf::Color(127, 127, 127));
		hideSecondPlayer.setPosition(8, 152);
		target.draw(hideSecondPlayer, states);
	}
	states.texture = &mTileset;
	target.draw(mVertices, states);
}

void StatisticsNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	mFirstPlayerText.setString(getLifesString(mPlayers[0].getPlayerData()));
	mSecondPlayerText.setString(getLifesString(mPlayers[1].getPlayerData()));
	construct();
}

std::string StatisticsNode::setLevelString(int level) const
{
	return countDigit(level) == 1 ? " " + std::to_string(level) : std::to_string(level);
}

std::string StatisticsNode::getLifesString(const PlayerData& playerData) const
{
	return playerData.canSpawn() ? std::to_string(playerData.getLifes()) : "0";
}