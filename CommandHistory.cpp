#include "CommandHistory.h"

void CommandHistory::push(
    std::unique_ptr<Command> command
)
{
    if (command != nullptr)
    {
        commands.push_back(
            std::move(command)
        );
    }
}

bool CommandHistory::undo()
{
    if (commands.empty())
    {
        return false;
    }

    commands.back()->undo();
    commands.pop_back();

    return true;
}

bool CommandHistory::empty() const
{
    return commands.empty();
}

void CommandHistory::clear()
{
    commands.clear();
}
