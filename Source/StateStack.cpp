#include "StateStack.hpp"
#include <cassert>

StateStack::StateStack(State::Context context) :
    mStack(),
    mPendingList(),
    mContext(context),
    mFactories()
{

}

void StateStack::update(sf::Time deltaTime)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(deltaTime))
        {
            break;
        }
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    for (State::Ptr &state : mStack)
    {
        state->draw();
    }
}

void StateStack::handleEvent(const std::optional<sf::Event> &event)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
        {
            break;
        }
    }
    applyPendingChanges();
}

void StateStack::pushState(States stateID)
{
    mPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

State::Ptr StateStack::createState(States stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange change : mPendingList)
    {
        switch (change.action)
        {
            case Action::Push:
            {
                mStack.push_back(createState(change.stateID));
                break;
            }
            case Action::Pop:
            {
                mStack.pop_back();
                break;
            }
            case Action::Clear:
            {
                mStack.clear();
                break;
            }
        }
    }
    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States stateID) :
    action(action),
    stateID(stateID)
{

}