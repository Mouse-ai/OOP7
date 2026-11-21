#include "CSquare.h"

CSquare::CSquare(int x, int y)
    : CRectangle(x, y, 60, 60)
{
}

CSquare::CSquare(int x, int y, int size)
    : CRectangle(x, y, size, size)
{
}

void CSquare::save(QTextStream& out, int indent) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "SQUARE "
        << x << " "
        << y << " "
        << width << " "
        << color.name()
        << "\n";
}
bool CSquare::load(
    QTextStream& in,
    const ShapeFactory&
)
{
    int newX;
    int newY;
    int newSize;
    QString colorText;

    in >> newX
       >> newY
       >> newSize
       >> colorText;

    QColor newColor(colorText);

    if (in.status() != QTextStream::Ok ||
        newSize < 20 ||
        !newColor.isValid())
    {
        return false;
    }

    x = newX;
    y = newY;

    width = newSize;
    height = newSize;

    color = newColor;
    selected = false;

    return true;
}