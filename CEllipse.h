#ifndef OOP4_CELLIPSE_H
#define OOP4_CELLIPSE_H

#include "Shape.h"

class CEllipse : public Shape
{
protected:
    int radiusX;
    int radiusY;

    bool canResize(int delta) const override;
    void changeSize(int delta) override;


public:
    CEllipse(int x, int y);
    CEllipse(int x, int y, int radiusX, int radiusY);

    bool contains(int pointX, int pointY) const override;
    void draw(QPainter &painter) const override;
    QRect boundingRect() const override;

    void save(QTextStream& out, int indent = 0) const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
    QString typeName() const override
    {
        return "Ellipse";
    }
};
#endif