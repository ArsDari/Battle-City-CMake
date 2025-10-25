#include "DataTables.hpp"
#include "Tank.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Command.hpp"

std::vector<TankData> initializeTankData()
{
    std::vector<TankData> data(static_cast<unsigned int>(Tank::Type::TypeCount));
    data[static_cast<unsigned int>(Tank::Type::FirstPlayerTank)].hitpoints = 1;
    data[static_cast<unsigned int>(Tank::Type::FirstPlayerTank)].framesPerMove = 4;
    data[static_cast<unsigned int>(Tank::Type::FirstPlayerTank)].texture = Textures::FirstPlayerTank;
    data[static_cast<unsigned int>(Tank::Type::FirstPlayerTank)].fireInterval = sf::seconds(12.f / 60.f);
    data[static_cast<unsigned int>(Tank::Type::SecondPlayerTank)].hitpoints = 1;
    data[static_cast<unsigned int>(Tank::Type::SecondPlayerTank)].framesPerMove = 4;
    data[static_cast<unsigned int>(Tank::Type::SecondPlayerTank)].texture = Textures::SecondPlayerTank;
    data[static_cast<unsigned int>(Tank::Type::SecondPlayerTank)].fireInterval = sf::seconds(12.f / 60.f);
    data[static_cast<unsigned int>(Tank::Type::EnemyLightTank)].hitpoints = 1;
    data[static_cast<unsigned int>(Tank::Type::EnemyLightTank)].framesPerMove = 2;
    data[static_cast<unsigned int>(Tank::Type::EnemyLightTank)].texture = Textures::EnemyLightTank;
    data[static_cast<unsigned int>(Tank::Type::EnemyLightTank)].fireInterval = sf::seconds(12.f / 60.f);
    data[static_cast<unsigned int>(Tank::Type::EnemyAPC)].hitpoints = 1;
    data[static_cast<unsigned int>(Tank::Type::EnemyAPC)].framesPerMove = 1;
    data[static_cast<unsigned int>(Tank::Type::EnemyAPC)].texture = Textures::EnemyAPC;
    data[static_cast<unsigned int>(Tank::Type::EnemyAPC)].fireInterval = sf::seconds(12.f / 60.f);
    data[static_cast<unsigned int>(Tank::Type::EnemyTankDestroyer)].hitpoints = 1;
    data[static_cast<unsigned int>(Tank::Type::EnemyTankDestroyer)].framesPerMove = 2;
    data[static_cast<unsigned int>(Tank::Type::EnemyTankDestroyer)].texture = Textures::EnemyTankDestroyer;
    data[static_cast<unsigned int>(Tank::Type::EnemyTankDestroyer)].fireInterval = sf::seconds(12.f / 60.f);
    data[static_cast<unsigned int>(Tank::Type::EnemyHeavyTank)].hitpoints = 4;
    data[static_cast<unsigned int>(Tank::Type::EnemyHeavyTank)].framesPerMove = 2;
    data[static_cast<unsigned int>(Tank::Type::EnemyHeavyTank)].texture = Textures::EnemyHeavyTank;
    data[static_cast<unsigned int>(Tank::Type::EnemyHeavyTank)].fireInterval = sf::seconds(12.f / 60.f);
    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(static_cast<unsigned int>(Projectile::Type::TypeCount));
    data[static_cast<unsigned int>(Projectile::Type::FirstPlayerSlowBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::FirstPlayerSlowBullet)].pixelPerFrame = 2;
    data[static_cast<unsigned int>(Projectile::Type::FirstPlayerFastBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::FirstPlayerFastBullet)].pixelPerFrame = 4;
    data[static_cast<unsigned int>(Projectile::Type::SecondPlayerSlowBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::SecondPlayerSlowBullet)].pixelPerFrame = 2;
    data[static_cast<unsigned int>(Projectile::Type::SecondPlayerFastBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::SecondPlayerFastBullet)].pixelPerFrame = 4;
    data[static_cast<unsigned int>(Projectile::Type::EnemySlowBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::EnemySlowBullet)].pixelPerFrame = 2;
    data[static_cast<unsigned int>(Projectile::Type::EnemyFastBullet)].damage = 1;
    data[static_cast<unsigned int>(Projectile::Type::EnemyFastBullet)].pixelPerFrame = 4;
    return data;
}

std::vector<PickupData> initializePickupData()
{
    sf::Vector2i spriteSize{ 16, 16 };
    std::vector<PickupData> data(static_cast<unsigned int>(Pickup::Type::TypeCount));
    data[static_cast<unsigned int>(Pickup::Type::Helmet)].action = [](Tank &tank) { tank.makeInvincible(sf::seconds(10.f)); };
    data[static_cast<unsigned int>(Pickup::Type::Helmet)].textureRect = sf::IntRect({ 0, 0 }, spriteSize);
    data[static_cast<unsigned int>(Pickup::Type::Clock)].action = [](Tank &tank) {
        Command command;
        command.category = Category::EnemyTank;
        command.action = derivedAction<Tank>([](Tank &tank, sf::Time) {tank.stopTank(sf::seconds(10.f), true); });
        tank.setPendingCommand(command);
    };
    data[static_cast<unsigned int>(Pickup::Type::Clock)].textureRect = sf::IntRect({ 16, 0 }, spriteSize);
    data[static_cast<unsigned int>(Pickup::Type::Shovel)].action = [](Tank &tank) { tank.defendBase(); };
    data[static_cast<unsigned int>(Pickup::Type::Shovel)].textureRect = sf::IntRect({ 32, 0 }, spriteSize);
    data[static_cast<unsigned int>(Pickup::Type::Star)].action = [](Tank &tank) { tank.upgrade(); };
    data[static_cast<unsigned int>(Pickup::Type::Star)].textureRect = sf::IntRect({ 48, 0 }, spriteSize);
    data[static_cast<unsigned int>(Pickup::Type::Grenade)].action = [](Tank &tank) {
        Command command;
        command.category = Category::EnemyTank;
        command.action = derivedAction<Tank>([](Tank &tank, sf::Time deltaTime) {tank.destroy(); });
        tank.setPendingCommand(command);
    };
    data[static_cast<unsigned int>(Pickup::Type::Grenade)].textureRect = sf::IntRect({64, 0}, spriteSize);
    data[static_cast<unsigned int>(Pickup::Type::Life)].action = [](Tank &tank) { tank.addLife(); };
    data[static_cast<unsigned int>(Pickup::Type::Life)].textureRect = sf::IntRect({ 80, 0 }, spriteSize);
    return data;
}