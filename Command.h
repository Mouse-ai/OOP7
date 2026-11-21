#ifndef OOP7_COMMAND_H
#define OOP7_COMMAND_H

class Command
{
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;
};

#endif