//
// Created by alexei on 24.10.2026.
//

#include "Shape.h"
#include <algorithm>
#include "Group.h"
Shape::Shape(int x, int y): x(x), y(y), color(Qt::black), selected(false){}
bool Shape::isSelected() const {
    return selected;
}
void Shape::setSelected(bool value) {
    selected = value;
}
void Shape:: toggleSelected() {
    selected = !selected;
}
void Shape::setColor(const QColor &newColor) {
    color = newColor;
}
void Shape::move(
    int dx,
    int dy,
    const QRect& bounds
)
{
    MoveContext context;

    moveWithContext(
        dx,
        dy,
        bounds,
        context
    );
}
bool Shape::resize(int delta, const QRect &bounds)
{
    if (!canResize(delta))
    {
        return false;
    }

    changeSize(delta);

    if (!fitsInBounds(bounds))
    {
        changeSize(-delta);
        return false;
    }

    return true;
}
bool Shape::fitsInBounds(const QRect &bounds) const
{
    return bounds.contains(boundingRect());
}
void Shape::subscribeToMovement(ShapeMoveObserver* observer)
{
    if (observer == nullptr)
    {
        return;
    }

    if (std::find(
            movementObservers.begin(),
            movementObservers.end(),
            observer
        ) == movementObservers.end())
    {
        movementObservers.push_back(observer);
    }
}
void Shape::unsubscribeFromMovement(ShapeMoveObserver* observer)
{
    movementObservers.erase(
        std::remove(
            movementObservers.begin(),
            movementObservers.end(),
            observer
        ),
        movementObservers.end()
    );
}
void Shape::notifyMoveObservers(
    int dx,
    int dy,
    const QRect& bounds,
    MoveContext& context
)
{
    for (ShapeMoveObserver* observer : movementObservers)
    {
        observer->onShapeMoved(
            this,
            dx,
            dy,
            bounds,
            context
        );
    }
}
bool Shape::moveWithContext(
    int dx,
    int dy,
    const QRect& bounds,
    MoveContext& context
)
{
    if (context.visited.contains(this))
    {
        return false;
    }

    context.visited.insert(this);

    x += dx;
    y += dy;

    if (!fitsInBounds(bounds))
    {
        x -= dx;
        y -= dy;

        return false;
    }

    notifyMoveObservers(
        dx,
        dy,
        bounds,
        context
    );

    return true;
}
void Shape::setParentGroup(Group* group)
{
    parentGroup = group;
}

Group* Shape::getParentGroup() const
{
    return parentGroup;
}
const Shape* Shape::getTopLevelOwner() const
{
    const Shape* current = this;

    while (current->parentGroup != nullptr)
    {
        current = current->parentGroup;
    }

    return current;
}
Shape* Shape::getTopLevelOwner()
{
    Shape* current = this;

    while (current->parentGroup != nullptr)
    {
        current = current->parentGroup;
    }

    return current;
}