#pragma once

namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		FirstPlayerTank = 1 << 1,
		SecondPlayerTank = 1 << 2,
		EnemyTank = 1 << 3,
		Pickup = 1 << 4,
		FirstPlayerProjectile = 1 << 5,
		SecondPlayerProjectile = 1 << 6,
		EnemyProjectile = 1 << 7,
		Tank = FirstPlayerTank | SecondPlayerTank | EnemyTank,
		Projectile = FirstPlayerProjectile | SecondPlayerProjectile | EnemyProjectile,
		PlayerTanks = FirstPlayerTank | SecondPlayerTank,
		PlayerProjectiles = FirstPlayerProjectile | SecondPlayerProjectile
	};
}