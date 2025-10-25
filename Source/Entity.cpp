#include "Entity.hpp"
#include <cassert>

Entity::Entity(int hitpoints) :
    mHitpoints(hitpoints)
{

}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::repair(int points)
{
    assert(points > 0);
    mHitpoints += points;
}

void Entity::damage(int points)
{
    assert(points > 0);
    mHitpoints -= points;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

void Entity::remove()
{
    destroy();
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}