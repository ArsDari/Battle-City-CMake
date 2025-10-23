#include "TileNode.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

TileNode::TileNode(TextureHolder& textures, Tilemap& tilemap, bool isLowerLayer) :
	mTileset(textures.get(Textures::Tileset)),
	mWallTileSize(8, 8),
	mVertices(sf::Quads, 26 * 26 * 4),
	mTiles(),
	mSmallExplosion(textures.get(Textures::SmallExplosionFrames), sf::Vector2i(16, 16), 2, sf::seconds(0.24f)),
	mBigExplosion(textures.get(Textures::BigExplosionFrames), sf::Vector2i(32, 32), 3, sf::seconds(0.36f)),
	mTilemap(tilemap),
	mIsLowerLayer(isLowerLayer)
{
	mSmallExplosion.setPosition(12 * 8, 24 * 8);
	mBigExplosion.setPosition(11 * 8, 23 * 8);
}

void TileNode::addLayer(Tilemap::Type layer)
{
	mTiles.push_back(layer);
}

void TileNode::construct()
{
	for (const auto& tile : mTilemap.get())
	{
		Tilemap::Type tileNumber = tile.tileNumber;
		if (std::find(mTiles.begin(), mTiles.end(), tileNumber) == mTiles.end())
			continue;
		Tilemap::Modifier modifier = tile.modifier;
		float x = static_cast<float>(tile.x);
		float y = static_cast<float>(tile.y);
		float tu = static_cast<float>(tileNumber % (mTileset.getSize().x / mWallTileSize.x));
		float tv = static_cast<float>(tileNumber / (mTileset.getSize().x / mWallTileSize.x));
		sf::Vertex* quad = &mVertices[static_cast<size_t>(x + y * 26) * 4];
		quad[0].position = sf::Vector2f(x * mWallTileSize.x, y * mWallTileSize.y);
		quad[1].position = sf::Vector2f((x + 1) * mWallTileSize.x, y * mWallTileSize.y);
		quad[2].position = sf::Vector2f((x + 1) * mWallTileSize.x, (y + 1) * mWallTileSize.y);
		quad[3].position = sf::Vector2f(x * mWallTileSize.x, (y + 1) * mWallTileSize.y);
		quad[0].texCoords = sf::Vector2f(tu * mWallTileSize.x, (tv + tile.modifier) * mWallTileSize.y);
		quad[1].texCoords = sf::Vector2f((tu + 1) * mWallTileSize.x, (tv + tile.modifier) * mWallTileSize.y);
		quad[2].texCoords = sf::Vector2f((tu + 1) * mWallTileSize.x, ((tv + tile.modifier) + 1) * mWallTileSize.y);
		quad[3].texCoords = sf::Vector2f(tu * mWallTileSize.x, ((tv + tile.modifier) + 1) * mWallTileSize.y);
	}
}

void TileNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mTilemap.isBaseDestroyed() && !mIsLowerLayer)
	{
		if (!mSmallExplosion.isFinished())
			target.draw(mSmallExplosion, states);
		else if (!mBigExplosion.isFinished())
			target.draw(mBigExplosion, states);
	}
	states.texture = &mTileset;
	target.draw(mVertices, states);
}

void TileNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{
	if (mTilemap.isBaseDestroyed() && !mIsLowerLayer)
	{
		if (!mSmallExplosion.isFinished())
			mSmallExplosion.update(deltaTime);
		else if (!mBigExplosion.isFinished())
			mBigExplosion.update(deltaTime);
	}
	construct();
}