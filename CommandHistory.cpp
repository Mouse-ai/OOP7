#include "CommandHistory.h"

bool CommandHistory::execute(
    std::unique_ptr<Command> command
)
{
    if (command == nullptr ||
        !command->execute())
    {
        return false;
    }

    commands.push_back(
        std::move(command)
    );

    return true;
}

bool CommandHistory::undo()
{
    if (commands.empty())
    {
        return false;
    }

    if (!commands.back()->undo())
    {
        return false;
    }

    commands.pop_back();

    return true;
}

bool CommandHistory::empty() const
{
    return commands.empty();
}
