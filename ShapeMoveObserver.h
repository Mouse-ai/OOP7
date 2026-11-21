#ifndef OOP7_SHAPEMOVEOBSERVER_H
#define OOP7_SHAPEMOVEOBSERVER_H

#include <QRect>

class Shape;
struct MoveContext;

class ShapeMoveObserver
{
public:
    virtual ~ShapeMoveObserver() = default;

    virtual void onShapeMoved(
        Shape* source,
        int dx,
        int dy,
        const QRect& bounds,
        MoveContext& context
    ) = 0;
};

#endif