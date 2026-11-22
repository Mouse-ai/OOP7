#include "CLine.h"

CLine::CLine(int x, int y)
    : CLine(x, y, 40)
{
}

CLine::CLine(int x, int y, int halfLength)
    : Shape(x, y),
      halfLength(halfLength)
{
}

bool CLine::contains(int pointX, int pointY) const
{
    const int tolerance = 5;

    bool insideX =
        pointX >= x - halfLength &&
        pointX <= x + halfLength;

    bool closeToLine =
        pointY >= y - tolerance &&
        pointY <= y + tolerance;

    return insideX && closeToLine;
}

void CLine::draw(QPainter &painter) const
{
    painter.save();

    QPen pen;
    pen.setColor(selected ? Qt::red : color);

    pen.setWidth(3);
    painter.setPen(pen);

    painter.drawLine(
        x - halfLength,
        y,
        x + halfLength,
        y
    );

    painter.restore();
}

QRect CLine::boundingRect() const
{
    const int margin = 2;

    return QRect(
        x - halfLength,
        y - margin,
        halfLength * 2,
        margin * 2
    );
}

bool CLine::canResize(int delta) const
{
    return halfLength + delta >= 10;
}

void CLine::changeSize(int delta)
{
    halfLength += delta;
}
void CLine::save(QTextStream& out, int indent) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "LINE "
        << x << " "
        << y << " "
        << halfLength << " "
        << color.name()
        << "\n";
}
bool CLine::load(
    QTextStream& in,
    const ShapeFactory&
)
{
    int newX;
    int newY;
    int newHalfLength;
    QString colorText;

    in >> newX
       >> newY
       >> newHalfLength
       >> colorText;

    QColor newColor(colorText);

    if (in.status() != QTextStream::Ok ||
        newHalfLength < 10 ||
        !newColor.isValid())
    {
        return false;
    }

    x = newX;
    y = newY;
    halfLength = newHalfLength;
    color = newColor;
    selected = false;

    return true;
}