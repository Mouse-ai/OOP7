#include "SnapshotCommand.h"

#include "ShapeStorage.h"
#include "ShapeFactory.h"

SnapshotCommand::SnapshotCommand(
    ShapeStorage& storage,
    const ShapeFactory& factory,
    const QRect& bounds,
    const QString& beforeState,
    const QString& afterState
)
    : storage(storage),
      factory(factory),
      bounds(bounds),
      beforeState(beforeState),
      afterState(afterState)
{
}

void SnapshotCommand::execute()
{
    storage.restoreSnapshot(
        afterState,
        factory,
        bounds
    );
}

void SnapshotCommand::undo()
{
    storage.restoreSnapshot(
        beforeState,
        factory,
        bounds
    );
}