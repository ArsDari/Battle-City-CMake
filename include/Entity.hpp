#pragma once

#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	enum Rotation
	{
		Left,
		Right,
		Up,
		Down,
		RotationCount
	};
	explicit Entity(int hitpoints);
	int getHitpoints() const;
	virtual void repair(int points);
	virtual void damage(int points);
	virtual void destroy();
	virtual void remove();
	virtual bool isDestroyed() const;
private:
	int mHitpoints;
};