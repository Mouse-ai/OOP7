//
// Created by alexei on 14.11.2026.
//

#ifndef OOP6_GROUP_H
#define OOP6_GROUP_H

#include "Shape.h"
#include <vector>

class Group : public Shape
{
private:
    std::vector<Shape*> children;
protected:
    bool canResize(int delta) const override;
    void changeSize(int delta) override;
public:
    Group();
    ~Group() override;

    void add(Shape* shape);

    void draw(QPainter& painter) const override;
    bool contains(int pointX, int pointY) const override;
    QRect boundingRect() const override;
    const std::vector<Shape*>& getChildren() const;
    void move(int dx, int dy, const QRect& bounds) override;
    std::vector<Shape*> takeChildren();
    void save(QTextStream& out, int indent = 0) const override;
    bool load(QTextStream& in,const ShapeFactory& factory) override;
    void setColor(const QColor& newColor) override;
    bool resize(int delta, const QRect& bounds) override;
    QString typeName() const override
    {
        return "Group";
    }
};

#endif

