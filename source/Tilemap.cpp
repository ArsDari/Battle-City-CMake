#include "Tilemap.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <sstream>

Tilemap::Tile::Tile(Type tileNumber, Modifier modifier, int x, int y) :
	tileNumber(tileNumber),
	modifier(modifier),
	x(x),
	y(y)
{

}

void Tilemap::Tile::updateTile(Type newTileNumber, Modifier newModifier)
{
	tileNumber = newTileNumber;
	modifier = newModifier;
}

Tilemap::Tilemap() :
	mDefenseTimeLeft(),
	mDefenseTickTime(),
	mWaterAnimationTime(),
	mWaterCurrentFrame(WaterFirstFrame),
	mIsBaseDestroyed(false),
	mIsBaseDefensed(false),
	mDefenseTick(false)
{
	mTilemap.reserve(26 * 26);
}

void Tilemap::load(const std::string& filename)
{
	std::ifstream link(filename);
	if (link.is_open())
	{
		std::string temp;
		int x = 0, y = 0;
		while (std::getline(link, temp))
		{
			for (char tileNumber : temp)
			{
				mTilemap.emplace_back(static_cast<Tilemap::Type>(tileNumber - '0'), Full, x, y);
				++x;
			}
			x = 0;
			++y;
		}
		assert(mTilemap.size() == 26 * 26);
		mTilemap[0 + 0 * 26].updateTile(Air); mTilemap[1 + 0 * 26].updateTile(Air);
		mTilemap[0 + 1 * 26].updateTile(Air); mTilemap[1 + 1 * 26].updateTile(Air);
		mTilemap[12 + 0 * 26].updateTile(Air); mTilemap[13 + 0 * 26].updateTile(Air);
		mTilemap[12 + 1 * 26].updateTile(Air); mTilemap[13 + 1 * 26].updateTile(Air);
		mTilemap[24 + 0 * 26].updateTile(Air); mTilemap[25 + 0 * 26].updateTile(Air);
		mTilemap[24 + 1 * 26].updateTile(Air); mTilemap[25 + 1 * 26].updateTile(Air);
		mTilemap[12 + 24 * 26].updateTile(FirstHalfOfBase); mTilemap[13 + 24 * 26].updateTile(SecondHalfOfBase);
		mTilemap[12 + 25 * 26].updateTile(FirstHalfOfBase, LowerHalfOfBase); mTilemap[13 + 25 * 26].updateTile(SecondHalfOfBase, LowerHalfOfBase);
	}
	else
		throw std::runtime_error("Cannot find the game map");
}

void Tilemap::load(const std::vector<Tile> tilemap)
{
	mTilemap = tilemap;
}

std::vector<Tilemap::Tile>& Tilemap::get()
{
	return mTilemap;
}

Tilemap::Tile& Tilemap::operator[](int index)
{
	return mTilemap[index];
}

Tilemap::Tile Tilemap::operator[](int index) const
{
	return mTilemap[index];
}

bool Tilemap::isBaseDestroyed() const
{
	return mIsBaseDestroyed;
}

void Tilemap::destroyBase()
{
	mTilemap[12 + 24 * 26].updateTile(DestroyedFirstHalfOfBase); mTilemap[13 + 24 * 26].updateTile(DestroyedSecondHalfOfBase);
	mTilemap[12 + 25 * 26].updateTile(DestroyedFirstHalfOfBase, LowerHalfOfBase); mTilemap[13 + 25 * 26].updateTile(DestroyedSecondHalfOfBase, LowerHalfOfBase);
	mIsBaseDestroyed = true;
}

void Tilemap::defendBase()
{
	if (!mIsBaseDefensed)
	{
		mDefenseTimeLeft = sf::seconds(20.f);
		mIsBaseDefensed = true;
		mDefenseTick = true;
		reinforceBase();
	}
}

void Tilemap::update(sf::Time deltaTime)
{
	if (mWaterAnimationTime > sf::seconds(12.f / 60.f))
	{
		mWaterAnimationTime = sf::Time::Zero;
		mWaterCurrentFrame = mWaterCurrentFrame + 1 > WaterThirdFrame ? WaterFirstFrame : static_cast<Modifier>(mWaterCurrentFrame + 1);
		for (auto& tile : mTilemap)
		{
			if (tile.tileNumber == Water)
				tile.modifier = mWaterCurrentFrame;
		}
	}
	else
		mWaterAnimationTime += deltaTime;
	if (mIsBaseDefensed)
	{
		if (mDefenseTimeLeft <= sf::Time::Zero)
		{
			mDefenseTick = false;
			mIsBaseDefensed = false;
			reinforceBase();
		}
		else if (sf::Time::Zero < mDefenseTimeLeft && mDefenseTimeLeft <= sf::seconds(4.f))
		{
			mDefenseTimeLeft -= deltaTime;
			if (mDefenseTickTime <= sf::Time::Zero)
			{
				mDefenseTickTime = sf::seconds(1.f);
				mDefenseTick = !mDefenseTick;
				reinforceBase();
			}
			else
				mDefenseTickTime -= deltaTime;
		}
		else
			mDefenseTimeLeft -= deltaTime;
	}
}

void Tilemap::reinforceBase()
{
	if (mDefenseTick)
	{
		mTilemap[11 + 23 * 26].updateTile(Concrete); mTilemap[12 + 23 * 26].updateTile(Concrete);
		mTilemap[13 + 23 * 26].updateTile(Concrete); mTilemap[14 + 23 * 26].updateTile(Concrete);
		mTilemap[11 + 24 * 26].updateTile(Concrete); mTilemap[14 + 24 * 26].updateTile(Concrete);
		mTilemap[11 + 25 * 26].updateTile(Concrete); mTilemap[14 + 25 * 26].updateTile(Concrete);
	}
	else
	{
		mTilemap[11 + 23 * 26].updateTile(Brick); mTilemap[12 + 23 * 26].updateTile(Brick);
		mTilemap[13 + 23 * 26].updateTile(Brick); mTilemap[14 + 23 * 26].updateTile(Brick);
		mTilemap[11 + 24 * 26].updateTile(Brick); mTilemap[14 + 24 * 26].updateTile(Brick);
		mTilemap[11 + 25 * 26].updateTile(Brick); mTilemap[14 + 25 * 26].updateTile(Brick);
	}
}

std::vector<std::string> Tilemap::split(const std::string& string, const char delimeter)
{
	std::vector<std::string> result;
	std::stringstream ss(string);
	std::string item;
	while (std::getline(ss, item, delimeter))
		result.push_back(item);
	return result;
}