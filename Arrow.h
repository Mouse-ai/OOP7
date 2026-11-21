#ifndef OOP7_ARROW_H
#define OOP7_ARROW_H

#include "ShapeMoveObserver.h"

class Shape;
class QPainter;

class Arrow : public ShapeMoveObserver
{
private:
    Shape* source;
    Shape* target;

public:
    Arrow(Shape* source, Shape* target);
    ~Arrow() override;

    Shape* getSource() const;
    Shape* getTarget() const;

    bool references(Shape* shape) const;

    void draw(QPainter& painter) const;

    void onShapeMoved(
        Shape* source,
        int dx,
        int dy,
        const QRect& bounds,
        MoveContext& context
    ) override;
};

#endif