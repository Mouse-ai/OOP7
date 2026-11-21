
#ifndef OOP4_CRECTANGLE_H
#define OOP4_CRECTANGLE_H

#include "Shape.h"

class CRectangle : public Shape
{
protected:
    int width;
    int height;

    bool canResize(int delta) const override;
    void changeSize(int delta) override;

public:
    CRectangle(int x, int y);
    CRectangle(int x, int y, int width, int height);

    bool contains(int pointX, int pointY) const override;
    void draw(QPainter &painter) const override;
    QRect boundingRect() const override;

    void save(QTextStream& out, int indent = 0) const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
    QString typeName() const override
    {
        return "Rectangle";
    }
};

#endif