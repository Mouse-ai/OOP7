#ifndef OOP6_SHAPEFACTORY_H
#define OOP6_SHAPEFACTORY_H

#include <QString>

class Shape;

class ShapeFactory
{
public:
    virtual ~ShapeFactory() = default;

    virtual Shape* create(const QString& type) const = 0;
};

class DefaultShapeFactory : public ShapeFactory
{
public:
    Shape* create(const QString& type) const override;
};

#endif