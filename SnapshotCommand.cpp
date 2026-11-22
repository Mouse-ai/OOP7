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

bool SnapshotCommand::execute()
{
    return storage.restoreSnapshot(
        afterState,
        factory,
        bounds
    );
}

bool SnapshotCommand::undo()
{
    return storage.restoreSnapshot(
        beforeState,
        factory,
        bounds
    );
}
