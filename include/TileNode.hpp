#pragma once

#include "Animation.hpp"
#include "SceneNode.hpp"
#include "Tilemap.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

class TileNode : public SceneNode
{
public:
	explicit TileNode(TextureHolder& textures, Tilemap& tilemap, bool isLowerLayer);
	void addLayer(Tilemap::Type tile);
	void construct();
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time deltaTime, CommandQueue& commands);
	sf::Texture mTileset;
	sf::Vector2i mWallTileSize;
	sf::VertexArray mVertices;
	std::vector<Tilemap::Type> mTiles;
	Animation mSmallExplosion;
	Animation mBigExplosion;
	Tilemap& mTilemap;
	bool mIsLowerLayer;
};