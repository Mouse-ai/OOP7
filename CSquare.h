#ifndef OOP4_CSQUARE_H
#define OOP4_CSQUARE_H

#include "CRectangle.h"

class CSquare : public CRectangle
{
public:
    CSquare(int x, int y);
    CSquare(int x, int y, int size);

    void save(QTextStream& out, int indent = 0) const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
};

#endif