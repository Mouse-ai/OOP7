#ifndef OOP7_SHAPE_H
#define OOP7_SHAPE_H

#include <QColor>
#include <QPainter>
#include <QRect>
#include <QTextStream>
#include <vector>
#include <unordered_set>
#include "ShapeMoveObserver.h"

class ShapeFactory;
class Shape;
class Group;

struct MoveContext
{
    std::unordered_set<Shape*> visited;
};

class Shape
{
private:
    std::vector<ShapeMoveObserver*> movementObservers;
    Group* parentGroup = nullptr;

protected:
    int x;
    int y;
    QColor color;
    bool selected;

    virtual bool canResize(int delta) const = 0;
    virtual void changeSize(int delta) = 0;

    void notifyMoveObservers(
        int dx,
        int dy,
        const QRect& bounds,
        MoveContext& context
    );

public:
    Shape(int x, int y);

    virtual ~Shape() = default;

    virtual void draw(QPainter &painter) const = 0;
    virtual bool contains(int pointX, int pointY) const = 0;

    bool isSelected() const;
    void setSelected(bool value);
    void toggleSelected();

    virtual void move(int dx, int dy, const QRect &bounds);

    virtual QRect boundingRect() const = 0;
    virtual void setColor(const QColor &newColor);
    virtual bool resize(int delta, const QRect &bounds);

    bool fitsInBounds(const QRect &bounds) const;

    virtual void save(QTextStream& out, int indent = 0) const = 0;

    virtual bool load(
        QTextStream& in,
        const ShapeFactory& factory
    ) = 0;

    virtual QString typeName() const = 0;

    void subscribeToMovement(ShapeMoveObserver* observer);
    void unsubscribeFromMovement(ShapeMoveObserver* observer);

    virtual bool moveWithContext(
        int dx,
        int dy,
        const QRect& bounds,
        MoveContext& context
    );

    void setParentGroup(Group* group);

    Shape* getTopLevelOwner();
    const Shape* getTopLevelOwner() const;
};

#endif
