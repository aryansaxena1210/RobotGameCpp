#pragma once
#include "RobotAgent.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class RobotAgentRoster
{
private:
    vector<RobotAgent *> agents;

public:
    RobotAgentRoster();
    ~RobotAgentRoster();

    void add(RobotAgent *agent);
    RobotAgent *operator[](int index);
    RobotAgent *operator[](const string &name);

    int size() const;
};