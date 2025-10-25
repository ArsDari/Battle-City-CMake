#pragma once

#include "ResourceIdentifiers.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Tank;

struct TankData
{
    int hitpoints = 0;
    int framesPerMove = 0;
    Textures texture = Textures::Unknown;
    sf::Time fireInterval;
};

struct ProjectileData
{
    int damage = 0;
    int pixelPerFrame = 0;
};

struct PickupData
{
    std::function<void(Tank&)> action;
    sf::IntRect textureRect;
};

std::vector<TankData> initializeTankData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();