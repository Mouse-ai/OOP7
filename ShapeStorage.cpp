//
// Created by alexei on 25.10.2026.
//

#include "ShapeStorage.h"
#include <QFile>
#include <QTextStream>
#include "ShapeFactory.h"
#include "ShapeStorage.h"
#include <algorithm>
#include "Arrow.h"
#include "Group.h"

void ShapeStorage::add(Shape *shape) {
    shapes.push_back(shape);
    notifyObservers();
}
int ShapeStorage::getCount() const {
    return static_cast<int>(shapes.size());
}
Shape *ShapeStorage::getObject(int index) const {
    return shapes[index];
}
void ShapeStorage::clearSelection() {
    for (Shape *shape : shapes) {
        shape->setSelected(false);
    }
}
ShapeStorage::~ShapeStorage()
{
    for (Arrow* arrow : arrows)
    {
        delete arrow;
    }

    for (Shape* shape : shapes)
    {
        delete shape;
    }
}

void ShapeStorage::removeSelected()
{
    size_t i = 0;

    while (i < shapes.size())
    {
        if (shapes[i]->isSelected())
        {
            removeArrowsReferencing(shapes[i]);

            delete shapes[i];
            shapes.erase(shapes.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    notifyObservers();
}
std::vector<Shape*> ShapeStorage::takeSelected()
{
    std::vector<Shape*> selectedShapes;

    int i = 0;

    while (i < shapes.size())
    {
        if (shapes[i]->isSelected())
        {
            selectedShapes.push_back(shapes[i]);
            shapes.erase(shapes.begin() + i);
        }
        else
        {
            ++i;
        }
    }

    return selectedShapes;
}
bool ShapeStorage::saveToFile(const QString& fileName) const
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    out << shapes.size() << "\n";

    for (Shape* shape : shapes)
    {
        shape->save(out);
    }

    return out.status() == QTextStream::Ok;
}
bool ShapeStorage::loadFromFile(
    const QString& fileName,
    const ShapeFactory& factory,
    const QRect& bounds
)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);

    int objectCount;

    in >> objectCount;

    if (in.status() != QTextStream::Ok ||
        objectCount < 0)
    {
        return false;
    }

    std::vector<Shape*> loadedShapes;

    for (int i = 0; i < objectCount; ++i)
    {
        QString type;

        in >> type;

        if (in.status() != QTextStream::Ok)
        {
            for (Shape* shape : loadedShapes)
            {
                delete shape;
            }

            return false;
        }

        Shape* shape = factory.create(type);

        if (shape == nullptr)
        {
            for (Shape* loadedShape : loadedShapes)
            {
                delete loadedShape;
            }

            return false;
        }

        if (!shape->load(in, factory))
        {
            delete shape;

            for (Shape* loadedShape : loadedShapes)
            {
                delete loadedShape;
            }

            return false;
        }

        if (!shape->fitsInBounds(bounds))
        {
            delete shape;

            for (Shape* loadedShape : loadedShapes)
            {
                delete loadedShape;
            }

            return false;
        }

        loadedShapes.push_back(shape);
    }

    for (Shape* shape : shapes)
    {
        delete shape;
    }

    shapes.clear();

    shapes.swap(loadedShapes);
    notifyObservers();
    return true;
}
void ShapeStorage::subscribe(StorageObserver* observer)
{
    if (observer == nullptr)
    {
        return;
    }

    observers.push_back(observer);
}

void ShapeStorage::unsubscribe(StorageObserver* observer)
{
    observers.erase(
        std::remove(
            observers.begin(),
            observers.end(),
            observer
        ),
        observers.end()
    );
}

void ShapeStorage::notifyObservers()
{
    for (StorageObserver* observer : observers)
    {
        observer->onStorageChanged();
    }
}
void ShapeStorage::addArrow(Arrow* arrow)
{
    if (arrow == nullptr)
    {
        return;
    }

    arrows.push_back(arrow);
}
int ShapeStorage::getArrowCount() const
{
    return static_cast<int>(arrows.size());
}
Arrow* ShapeStorage::getArrow(int index) const
{
    return arrows[index];
}
bool ShapeStorage::containsShape(
    Shape* root,
    Shape* searched
) const
{
    if (root == searched)
    {
        return true;
    }

    Group* group =
        dynamic_cast<Group*>(root);

    if (group == nullptr)
    {
        return false;
    }

    for (Shape* child : group->getChildren())
    {
        if (containsShape(child, searched))
        {
            return true;
        }
    }

    return false;
}
void ShapeStorage::removeArrowsReferencing(Shape* shape)
{
    size_t i = 0;

    while (i < arrows.size())
    {
        Shape* source =
            arrows[i]->getSource();

        Shape* target =
            arrows[i]->getTarget();

        bool sourceInside =
            containsShape(shape, source);

        bool targetInside =
            containsShape(shape, target);

        if (sourceInside || targetInside)
        {
            delete arrows[i];
            arrows.erase(arrows.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}