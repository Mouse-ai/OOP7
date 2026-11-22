
//
// Created by alexei on 25.10.2026.
//

#include "CRectangle.h"

CRectangle::CRectangle(int x, int y)
    : CRectangle(x, y, 80, 50)
{
}

CRectangle::CRectangle(
    int x,
    int y,
    int width,
    int height
)
    : Shape(x, y),
      width(width),
      height(height)
{
}

bool CRectangle::contains(
    int pointX,
    int pointY
) const
{
    int left = x - width / 2;
    int right = x + width / 2;
    int top = y - height / 2;
    int bottom = y + height / 2;

    return pointX >= left &&
           pointX <= right &&
           pointY >= top &&
           pointY <= bottom;
}

void CRectangle::draw(
    QPainter& painter
) const
{
    painter.save();

    QPen pen;
    pen.setColor(
        selected ? Qt::red : color
    );
    pen.setWidth(3);

    painter.setPen(pen);

    painter.drawRect(
        x - width / 2,
        y - height / 2,
        width,
        height
    );

    painter.restore();
}

QRect CRectangle::boundingRect() const
{
    return QRect(
        x - width / 2,
        y - height / 2,
        width,
        height
    );
}

bool CRectangle::canResize(
    int delta
) const
{
    return width + delta >= 20 &&
           height + delta >= 20;
}

void CRectangle::changeSize(
    int delta
)
{
    width += delta;
    height += delta;
}

void CRectangle::save(
    QTextStream& out,
    int indent
) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "RECTANGLE "
        << x << " "
        << y << " "
        << width << " "
        << height << " "
        << color.name()
        << "\n";
}

bool CRectangle::load(
    QTextStream& in,
    const ShapeFactory&
)
{
    int newX;
    int newY;
    int newWidth;
    int newHeight;
    QString colorText;

    in >> newX
       >> newY
       >> newWidth
       >> newHeight
       >> colorText;

    QColor newColor(colorText);

    if (in.status() != QTextStream::Ok ||
        newWidth < 20 ||
        newHeight < 20 ||
        !newColor.isValid())
    {
        return false;
    }

    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
    color = newColor;
    selected = false;

    return true;
}
