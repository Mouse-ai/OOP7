#ifndef OOP7_COMMAND_H
#define OOP7_COMMAND_H

class Command
{
public:
    virtual ~Command() = default;

    virtual bool execute() = 0;
    virtual bool undo() = 0;
};

#endif
