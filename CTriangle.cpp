//
// Created by alexei on 26.10.2026.
//

#include "CTriangle.h"

CTriangle::CTriangle(int x, int y)
    : CTriangle(x, y, 80, 70)
{
}

CTriangle::CTriangle(int x, int y, int width, int height)
    : Shape(x, y),
      width(width),
      height(height)
{
}

QPolygon CTriangle::getPolygon() const
{
    QPolygon polygon;
    // Формируем вершины из центра и текущего размера.
    // Этот же полигон используется для рисования и проверки попадания.
    polygon << QPoint(x, y - height / 2)
            << QPoint(x - width / 2, y + height / 2)
            << QPoint(x + width / 2, y + height / 2);

    return polygon;
}
bool CTriangle::contains(int pointX, int pointY) const
{
    return getPolygon().containsPoint(
        QPoint(pointX, pointY),
        Qt::OddEvenFill
    );
}
void CTriangle::draw(QPainter &painter) const
{
    painter.save();

    QPen pen;

    if (selected)
    {
        pen.setColor(Qt::red);
    }
    else
    {
        pen.setColor(color);
    }

    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawPolygon(getPolygon());

    painter.restore();
}
QRect CTriangle::boundingRect() const
{
    return QRect(
        x - width / 2,
        y - height / 2,
        width,
        height
    );
}
bool CTriangle::canResize(int delta) const
{
    return width + delta >= 20 &&
           height + delta >= 20;
}

void CTriangle::changeSize(int delta)
{
    width += delta;
    height += delta;
}
void CTriangle::save(QTextStream& out, int indent) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "TRIANGLE "
        << x << " "
        << y << " "
        << width << " "
        << height << " "
        << color.name()
        << "\n";
}
bool CTriangle::load(
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