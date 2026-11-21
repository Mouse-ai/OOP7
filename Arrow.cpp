#include "Arrow.h"
#include "Shape.h"

#include <QPainter>
#include <QPolygonF>
#include <cmath>

Arrow::Arrow(Shape* source, Shape* target)
    : source(source),
      target(target)
{
    if (source != nullptr)
    {
        source->subscribeToMovement(this);
    }
}

Arrow::~Arrow()
{
    if (source != nullptr)
    {
        source->unsubscribeFromMovement(this);
    }
}

Shape* Arrow::getSource() const
{
    return source;
}

Shape* Arrow::getTarget() const
{
    return target;
}

bool Arrow::references(Shape* shape) const
{
    return source == shape ||
           target == shape;
}
void Arrow::onShapeMoved(
    Shape* movedSource,
    int dx,
    int dy,
    const QRect& bounds,
    MoveContext& context
)
{
    if (movedSource != source ||
        target == nullptr)
    {
        return;
    }

    target->moveWithContext(
        dx,
        dy,
        bounds,
        context
    );
}
void Arrow::draw(QPainter& painter) const
{
    if (source == nullptr ||
        target == nullptr)
    {
        return;
    }

    QPointF start =
        source->boundingRect().center();

    QPointF end =
        target->boundingRect().center();

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();

    double length =
        std::sqrt(dx * dx + dy * dy);

    if (length < 1.0)
    {
        return;
    }

    double ux = dx / length;
    double uy = dy / length;

    const double arrowLength = 14.0;
    const double arrowWidth = 7.0;

    QPointF base(
        end.x() - ux * arrowLength,
        end.y() - uy * arrowLength
    );

    QPointF perpendicular(
        -uy,
        ux
    );

    QPointF left(
        base.x() + perpendicular.x() * arrowWidth,
        base.y() + perpendicular.y() * arrowWidth
    );

    QPointF right(
        base.x() - perpendicular.x() * arrowWidth,
        base.y() - perpendicular.y() * arrowWidth
    );

    painter.save();

    QPen pen(Qt::darkGray);
    pen.setWidth(2);

    painter.setPen(pen);
    painter.setBrush(Qt::darkGray);

    painter.drawLine(start, end);

    QPolygonF arrowHead;
    arrowHead << end
              << left
              << right;

    painter.drawPolygon(arrowHead);

    painter.restore();
}