#ifndef OOP4_CCIRCLE_H
#define OOP4_CCIRCLE_H

#include "CEllipse.h"

class CCircle : public CEllipse
{
public:
    CCircle(int x, int y);
    CCircle(int x, int y, int radius);

    void save(QTextStream& out, int indent = 0) const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
};

#endif