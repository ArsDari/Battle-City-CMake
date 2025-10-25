#pragma once

#include "Category.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <memory>
#include <utility>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable
{
public:
    SceneNode(const SceneNode &) = delete;
    SceneNode &operator=(const SceneNode &) = delete;
    using Ptr = std::unique_ptr<SceneNode>;
    using Pair = std::pair<SceneNode *, SceneNode *>;
    explicit SceneNode(Category category = Category::None);
    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode &node);
    void update(sf::Time deltaTime, CommandQueue &commands);
    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;
    void onCommand(const Command &command, sf::Time deltaTime);
    virtual Category getCategory() const;
    void checkSceneCollision(SceneNode &sceneGraph, std::set<Pair> &collisionPairs);
    void checkNodeCollision(SceneNode &node, std::set<Pair> &collisionPairs);
    void removeWrecks();
    virtual sf::FloatRect getBoundingRect() const;
    virtual bool isMarkedForRemoval() const;
    virtual bool isDestroyed() const;
private:
    virtual void updateCurrent(sf::Time deltaTime, CommandQueue &commands);
    void updateChildren(sf::Time deltaTime, CommandQueue &commands);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
    std::vector<Ptr> mChildren;
    SceneNode *mParent;
    Category mDefaultCategory;
};

bool collision(const SceneNode &lhs, const SceneNode &rhs);
sf::Vector2f distance(const SceneNode &lhs, const SceneNode &rhs);