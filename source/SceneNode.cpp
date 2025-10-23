#include "SceneNode.hpp"
#include "Command.hpp"
#include <algorithm>
#include <cassert>

SceneNode::SceneNode(Category::Type category) :
	mChildren(),
	mParent(nullptr),
	mDefaultCategory(category)
{

}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());
	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time deltaTime, CommandQueue& commands)
{
	updateCurrent(deltaTime, commands);
	updateChildren(deltaTime, commands);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;
	return transform;
}

void SceneNode::onCommand(const Command& command, sf::Time deltaTime)
{
	if (command.category & getCategory())
		command.action(*this, deltaTime);
	for (Ptr& child : mChildren)
		child->onCommand(command, deltaTime);
}

unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);
	for (Ptr& child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
		collisionPairs.insert(std::minmax(this, &node));
	for (Ptr& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::removeWrecks()
{
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());
	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	return false;
}

void SceneNode::updateCurrent(sf::Time deltaTime, CommandQueue& commands)
{

}

void SceneNode::updateChildren(sf::Time deltaTime, CommandQueue& commands)
{
	for (Ptr& child : mChildren)
		child->update(deltaTime, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Ptr& child : mChildren)
		child->draw(target, states);
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

sf::Vector2f distance(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getWorldPosition() - rhs.getWorldPosition();
}