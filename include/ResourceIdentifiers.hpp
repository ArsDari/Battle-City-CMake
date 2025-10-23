#pragma once

namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		FirstPlayer,
		SecondPlayer,
		EnemyRegularTank,
		EnemyAPC,
		EnemyRapidFireTank,
		EnemyHeavyTank,
		Tileset,
		BulletTile,
		Pickups,
		InvicibilityFrames,
		SpawningFrames,
		Points,
		SmallExplosionFrames,
		BigExplosionFrames,
		ButtonSelected,
		Statistics,
		TankMark,
		Logo,
		FirstPlayerScorePrefix,
		MostScorePrefix,
		SecondPlayerScorePrefix,
		Unknown
	};
}

namespace Fonts
{
	enum ID
	{
		Main
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures::ID>;
using FontHolder = ResourceHolder<sf::Font, Fonts::ID>;