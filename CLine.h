
#ifndef OOP4_CLINE_H
#define OOP4_CLINE_H

#include "Shape.h"

class CLine : public Shape
{
private:
    int halfLength;

protected:
    bool canResize(int delta) const override;
    void changeSize(int delta) override;

public:
    CLine(int x, int y);
    CLine(int x, int y, int halfLength);

    void save(QTextStream& out, int indent = 0) const override;

    bool contains(int pointX, int pointY) const override;
    void draw(QPainter &painter) const override;
    QRect boundingRect() const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
    QString typeName() const override
    {
        return "Line";
    }
};

#endif