#include "Container.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{
    Container::Container() :
        mChildren(),
        mSelectedChild(-1)
    {

    }

    void Container::pack(Component::Ptr component)
    {
        mChildren.push_back(component);
        if (!hasSelection() && component->isSelectable())
        {
            select(mChildren.size() - 1);
        }
    }

    bool Container::isSelectable() const
    {
        return false;
    }

    void Container::handleEvent(const std::optional<sf::Event> &event)
    {
        if (hasSelection() && mChildren[mSelectedChild]->isActive())
        {
            mChildren[mSelectedChild]->handleEvent(event);
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up)
            {
                selectPrevious();
            }
            else if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down)
            {
                selectNext();
            }
            else if (hasSelection() && keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space)
            {
                mChildren[mSelectedChild]->activate();
            }
        }
    }

    void Container::update(sf::Time deltaTime)
    {
        for (const Component::Ptr &child : mChildren)
            child->update(deltaTime);
    }

    void Container::setShowSprite(bool flag)
    {
        for (const Component::Ptr &child : mChildren)
            child->setShowSprite(flag);
    }

    void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        for (const Component::Ptr &child : mChildren)
            target.draw(*child, states);
    }

    bool Container::hasSelection() const
    {
        return mSelectedChild >= 0;
    }

    void Container::select(std::size_t index)
    {
        if (mChildren[index]->isSelectable())
        {
            if (hasSelection())
                mChildren[mSelectedChild]->deselect();
            mChildren[index]->select();
            mSelectedChild = index;
        }
    }

    void Container::selectNext()
    {
        if (!hasSelection())
        {
            return;
        }
        unsigned int next = mSelectedChild;
        do
        {
            next = (static_cast<unsigned long long>(next) + 1) % mChildren.size();
        }
        while (!mChildren[next]->isSelectable());
        select(next);
    }

    void Container::selectPrevious()
    {
        if (!hasSelection())
        {
            return;
        }
        int prev = mSelectedChild;
        do
        {
            prev = (prev + mChildren.size() - 1) % mChildren.size();
        }
        while (!mChildren[prev]->isSelectable());
        select(prev);
    }
}