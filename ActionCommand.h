#ifndef OOP7_ACTIONCOMMAND_H
#define OOP7_ACTIONCOMMAND_H

#include "Command.h"

#include <QRect>
#include <QString>
#include <functional>

class ShapeStorage;
class ShapeFactory;

class ActionCommand : public Command
{
private:
    ShapeStorage& storage;
    const ShapeFactory& factory;
    QRect bounds;
    std::function<void()> action;
    QString beforeState;
    QString afterState;

public:
    ActionCommand(
        ShapeStorage& storage,
        const ShapeFactory& factory,
        const QRect& bounds,
        std::function<void()> action
    );

    bool execute() override;
    bool undo() override;
};

#endif
