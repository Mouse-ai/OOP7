#ifndef OOP7_COMMANDHISTORY_H
#define OOP7_COMMANDHISTORY_H

#include "Command.h"

#include <memory>
#include <vector>

class CommandHistory
{
private:
    std::vector<std::unique_ptr<Command>> commands;

public:
    bool execute(std::unique_ptr<Command> command);
    bool undo();
    bool empty() const;
};

#endif
