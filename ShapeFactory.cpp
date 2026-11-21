#include "ShapeFactory.h"

#include "Ccircle.h"
#include "CEllipse.h"
#include "CRectangle.h"
#include "CSquare.h"
#include "CTriangle.h"
#include "CLine.h"
#include "Group.h"

Shape* DefaultShapeFactory::create(const QString& type) const
{
    if (type == "CIRCLE")
    {
        return new CCircle(0, 0);
    }

    if (type == "ELLIPSE")
    {
        return new CEllipse(0, 0);
    }

    if (type == "RECTANGLE")
    {
        return new CRectangle(0, 0);
    }

    if (type == "SQUARE")
    {
        return new CSquare(0, 0);
    }

    if (type == "TRIANGLE")
    {
        return new CTriangle(0, 0);
    }

    if (type == "LINE")
    {
        return new CLine(0, 0);
    }

    if (type == "GROUP")
    {
        return new Group();
    }

    return nullptr;
}