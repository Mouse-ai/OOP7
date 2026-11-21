#include "ActionCommand.h"

#include "ShapeFactory.h"
#include "ShapeStorage.h"

#include <utility>

ActionCommand::ActionCommand(
    ShapeStorage& storage,
    const ShapeFactory& factory,
    const QRect& bounds,
    std::function<void()> action
)
    : storage(storage),
      factory(factory),
      bounds(bounds),
      action(std::move(action))
{
}

bool ActionCommand::execute()
{
    beforeState = storage.createSnapshot();

    if (beforeState.isEmpty())
    {
        return false;
    }

    action();

    afterState = storage.createSnapshot();
    action = {};

    if (afterState.isEmpty())
    {
        storage.restoreSnapshot(
            beforeState,
            factory,
            bounds
        );

        return false;
    }

    return beforeState != afterState;
}

bool ActionCommand::undo()
{
    return storage.restoreSnapshot(
        beforeState,
        factory,
        bounds
    );
}
