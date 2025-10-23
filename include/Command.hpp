#pragma once

#include "Category.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
	using Action = std::function<void(SceneNode&, sf::Time)>;
	Command();
	Action action;
	unsigned int category;
};

template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time deltaTime)
		{
			assert(dynamic_cast<GameObject*>(&node) != nullptr);
			fn(static_cast<GameObject&>(node), deltaTime);
		};
}