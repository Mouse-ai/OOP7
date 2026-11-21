#include "Ccircle.h"

CCircle::CCircle(int x, int y)
    : CEllipse(x, y, 30, 30)
{
}

CCircle::CCircle(int x, int y, int radius)
    : CEllipse(x, y, radius, radius)
{
}

void CCircle::save(QTextStream& out, int indent) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "CIRCLE "
        << x << " "
        << y << " "
        << radiusX << " "
        << color.name()
        << "\n";
}
bool CCircle::load(
    QTextStream& in,
    const ShapeFactory&
)
{
    int newX;
    int newY;
    int newRadius;
    QString colorText;

    in >> newX
       >> newY
       >> newRadius
       >> colorText;

    QColor newColor(colorText);

    if (in.status() != QTextStream::Ok ||
        newRadius < 10 ||
        !newColor.isValid())
    {
        return false;
    }

    x = newX;
    y = newY;

    radiusX = newRadius;
    radiusY = newRadius;

    color = newColor;
    selected = false;

    return true;
}