#include "DataTables.hpp"
#include "Tank.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Command.hpp"

std::vector<TankData> initializeTankData()
{
	std::vector<TankData> data(Tank::TypeCount);
	data[Tank::FirstPlayer].hitpoints = 1;
	data[Tank::FirstPlayer].framesPerMove = 4;
	data[Tank::FirstPlayer].texture = Textures::FirstPlayer;
	data[Tank::FirstPlayer].fireInterval = sf::seconds(12.f / 60.f);
	data[Tank::SecondPlayer].hitpoints = 1;
	data[Tank::SecondPlayer].framesPerMove = 4;
	data[Tank::SecondPlayer].texture = Textures::SecondPlayer;
	data[Tank::SecondPlayer].fireInterval = sf::seconds(12.f / 60.f);
	data[Tank::EnemyRegularTank].hitpoints = 1;
	data[Tank::EnemyRegularTank].framesPerMove = 2;
	data[Tank::EnemyRegularTank].texture = Textures::EnemyRegularTank;
	data[Tank::EnemyRegularTank].fireInterval = sf::seconds(12.f / 60.f);
	data[Tank::EnemyAPC].hitpoints = 1;
	data[Tank::EnemyAPC].framesPerMove = 1;
	data[Tank::EnemyAPC].texture = Textures::EnemyAPC;
	data[Tank::EnemyAPC].fireInterval = sf::seconds(12.f / 60.f);
	data[Tank::EnemyRapidFireTank].hitpoints = 1;
	data[Tank::EnemyRapidFireTank].framesPerMove = 2;
	data[Tank::EnemyRapidFireTank].texture = Textures::EnemyRapidFireTank;
	data[Tank::EnemyRapidFireTank].fireInterval = sf::seconds(12.f / 60.f);
	data[Tank::EnemyHeavyTank].hitpoints = 4;
	data[Tank::EnemyHeavyTank].framesPerMove = 2;
	data[Tank::EnemyHeavyTank].texture = Textures::EnemyHeavyTank;
	data[Tank::EnemyHeavyTank].fireInterval = sf::seconds(12.f / 60.f);
	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);
	data[Projectile::FirstPlayerSlowBullet].damage = 1;
	data[Projectile::FirstPlayerSlowBullet].pixelPerFrame = 2;
	data[Projectile::FirstPlayerFastBullet].damage = 1;
	data[Projectile::FirstPlayerFastBullet].pixelPerFrame = 4;
	data[Projectile::SecondPlayerSlowBullet].damage = 1;
	data[Projectile::SecondPlayerSlowBullet].pixelPerFrame = 2;
	data[Projectile::SecondPlayerFastBullet].damage = 1;
	data[Projectile::SecondPlayerFastBullet].pixelPerFrame = 4;
	data[Projectile::EnemySlowBullet].damage = 1;
	data[Projectile::EnemySlowBullet].pixelPerFrame = 2;
	data[Projectile::EnemyFastBullet].damage = 1;
	data[Projectile::EnemyFastBullet].pixelPerFrame = 4;
	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	data[Pickup::Helmet].action = [](Tank& tank) { tank.makeInvincible(sf::seconds(10.f)); };
	data[Pickup::Helmet].textureRect = sf::IntRect(0, 0, 16, 16);
	data[Pickup::Clock].action = [](Tank& tank) {
			Command command;
			command.category = Category::EnemyTank;
			command.action = derivedAction<Tank>([](Tank& tank, sf::Time) {tank.stopTank(sf::seconds(10.f), true); });
			tank.setPendingCommand(command);
		};
	data[Pickup::Clock].textureRect = sf::IntRect(16, 0, 16, 16);
	data[Pickup::Shovel].action = [](Tank& tank) { tank.defendBase(); };
	data[Pickup::Shovel].textureRect = sf::IntRect(32, 0, 16, 16);
	data[Pickup::Star].action = [](Tank& tank) { tank.upgrade(); };
	data[Pickup::Star].textureRect = sf::IntRect(48, 0, 16, 16);
	data[Pickup::Grenade].action = [](Tank& tank) {
			Command command;
			command.category = Category::EnemyTank;
			command.action = derivedAction<Tank>([](Tank& tank, sf::Time deltaTime) {tank.destroy(); });
			tank.setPendingCommand(command);
		};
	data[Pickup::Grenade].textureRect = sf::IntRect(64, 0, 16, 16);
	data[Pickup::Life].action = [](Tank& tank) { tank.addLife(); };
	data[Pickup::Life].textureRect = sf::IntRect(80, 0, 16, 16);
	return data;
}