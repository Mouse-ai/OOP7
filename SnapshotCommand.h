#ifndef OOP7_SNAPSHOTCOMMAND_H
#define OOP7_SNAPSHOTCOMMAND_H

#include "Command.h"

#include <QString>
#include <QRect>

class ShapeStorage;
class ShapeFactory;

class SnapshotCommand : public Command
{
private:
    ShapeStorage& storage;
    const ShapeFactory& factory;

    QRect bounds;

    QString beforeState;
    QString afterState;

public:
    SnapshotCommand(
        ShapeStorage& storage,
        const ShapeFactory& factory,
        const QRect& bounds,
        const QString& beforeState,
        const QString& afterState
    );

    bool execute() override;
    bool undo() override;
};

#endif
