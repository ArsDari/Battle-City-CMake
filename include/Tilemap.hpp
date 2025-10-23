#pragma once

#include <vector>
#include <string>
#include <SFML/System/Time.hpp>

class Tilemap
{
public:
	enum Type
	{
		Air,
		Brick,
		Concrete,
		Leaf,
		Ice,
		Water,
		FirstHalfOfBase,
		SecondHalfOfBase,
		DestroyedFirstHalfOfBase,
		DestroyedSecondHalfOfBase
	};
	enum Modifier
	{
		Full = 0,
		BrickFull = 0,
		BrickLeft,
		BrickRight,
		BrickUp,
		BrickDown,
		BrickLeftUpperQuarter,
		BrickLeftLowerQuarter,
		BrickRightUpperQuarter,
		BrickRightLowerQuarter,
		WaterFirstFrame = 0,
		WaterSecondFrame,
		WaterThirdFrame,
		LowerHalfOfBase = 1
	};
	struct Tile
	{
		Tile(Type tileNumber, Modifier modifier, int x, int y);
		void updateTile(Type newTileNumber, Modifier newModifier = Full);
		Type tileNumber;
		Modifier modifier;
		int x, y;
	};
	Tilemap();
	void load(const std::string& filename);
	void load(const std::vector<Tile> tilemap);
	std::vector<Tile>& get();
	Tile& operator[](int index);
	Tile operator[](int index) const;
	bool isBaseDestroyed() const;
	void destroyBase();
	void defendBase();
	void update(sf::Time deltaTime);
private:
	void reinforceBase();
	std::vector<std::string> split(const std::string& string, const char delimeter);
	std::vector<Tile> mTilemap;
	sf::Time mDefenseTimeLeft;
	sf::Time mDefenseTickTime;
	sf::Time mWaterAnimationTime;
	Modifier mWaterCurrentFrame;
	bool mIsBaseDestroyed;
	bool mIsBaseDefensed;
	bool mDefenseTick;
};