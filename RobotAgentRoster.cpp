#include "RobotAgentRoster.h"
#include <iostream>

// Constructor - creates roster with required agents
// For now, we'll just create an empty roster since we haven't implemented the agents yet
RobotAgentRoster::RobotAgentRoster()
{
    // Will add LazyAgent, RandomAgent, etc. once they're implemented
    std::cout << "RobotAgentRoster created (agents will be added later)" << std::endl;
}

// Destructor - clean up dynamically allocated agents
RobotAgentRoster::~RobotAgentRoster()
{
    for (RobotAgent *agent : agents)
    {
        delete agent;
    }
    agents.clear();
}

// Add a robot agent to the roster
void RobotAgentRoster::add(RobotAgent *agent)
{
    if (agent != nullptr)
    {
        agents.push_back(agent);
    }
}

// Get agent by index
RobotAgent *RobotAgentRoster::operator[](int index)
{
    if (index >= 0 && index < static_cast<int>(agents.size()))
    {
        return agents[index];
    }
    return nullptr;
}

// Get agent by name
RobotAgent *RobotAgentRoster::operator[](const string &name)
{
    for (RobotAgent *agent : agents)
    {
        if (agent->getAgentName() == name)
        {
            return agent;
        }
    }
    return nullptr;
}

// Get size of roster
int RobotAgentRoster::size() const
{
    return static_cast<int>(agents.size());
}