#pragma once

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <functional>
#include <map>

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack
{
public:
    enum class Action
    {
        Push,
        Pop,
        Clear
    };
public:
    StateStack(const StateStack &) = delete;
    StateStack &operator=(const StateStack &) = delete;
    explicit StateStack(State::Context context);
    template <typename T>
    void registerState(States stateID);
    void update(sf::Time deltaTime);
    void draw();
    void handleEvent(const sf::Event &event);
    void pushState(States stateID);
    void popState();
    void clearStates();
    bool isEmpty() const;
private:
    State::Ptr createState(States stateID);
    void applyPendingChanges();
    struct PendingChange
    {
        explicit PendingChange(Action action, States stateID = States::None);
        Action action;
        States stateID;
    };
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    State::Context mContext;
    std::map<States, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States stateID)
{
    mFactories[stateID] = [this]() {
        return State::Ptr(new T(*this, mContext));
    };
}