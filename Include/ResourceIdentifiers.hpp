#pragma once

namespace sf
{
    class Texture;
    class Font;
}

enum class Textures
{
    Titlescreen,
    FirstPlayerTank,
    SecondPlayerTank,
    EnemyLightTank,
    EnemyAPC,
    EnemyTankDestroyer,
    EnemyHeavyTank,
    BrickTileset,
    Concrete,
    WaterAnimation,
    Leaves,
    Ice,
    Eagle,
    DestroyedEagle,
    BulletTileset,
    Pickups,
    Points,
    InvicibilityAnimation,
    SpawnAnimation,
    SmallExplosionAnimation,
    BigExplosionAnimation,
    FirstPlayerStatistics,
    SecondPlayerStatistics,
    HighScoreStatistics,
    ButtonSelect,
    Statistics,
    IconTank,
    EasterEgg,
    Unknown
};

enum class Fonts
{
    Main
};

template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder = ResourceHolder<sf::Texture, Textures>;
using FontHolder = ResourceHolder<sf::Font, Fonts>;