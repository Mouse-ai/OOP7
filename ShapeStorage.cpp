
#include "ShapeStorage.h"

#include "Arrow.h"
#include "Group.h"
#include "ShapeFactory.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

#include <algorithm>


void ShapeStorage::add(Shape* shape)
{
    shapes.push_back(shape);

    notifyObservers();
}


int ShapeStorage::getCount() const
{
    return static_cast<int>(shapes.size());
}


Shape* ShapeStorage::getObject(int index) const
{
    return shapes[index];
}


void ShapeStorage::clearSelection()
{
    for (Shape* shape : shapes)
    {
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
    bool changed = false;
    size_t i = 0;

    while (i < shapes.size())
    {
        if (shapes[i]->isSelected())
        {
            removeArrowsReferencing(shapes[i]);

            delete shapes[i];
            shapes.erase(shapes.begin() + i);
            changed = true;
        }
        else
        {
            ++i;
        }
    }

    if (changed)
    {
        notifyObservers();
    }
}


std::vector<Shape*> ShapeStorage::takeSelected()
{
    std::vector<Shape*> selectedShapes;

    size_t i = 0;

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

bool ShapeStorage::saveToStream(
    QTextStream& out
) const
{
    out << shapes.size() << "\n";

    for (Shape* shape : shapes)
    {
        shape->save(out);
    }

    out << "ARROWS "
        << arrows.size()
        << "\n";

    for (Arrow* arrow : arrows)
    {
        QString sourcePath =
            findPath(arrow->getSource());

        QString targetPath =
            findPath(arrow->getTarget());

        if (sourcePath.isEmpty() ||
            targetPath.isEmpty())
        {
            return false;
        }

        out << sourcePath
            << " "
            << targetPath
            << "\n";
    }

    return out.status() == QTextStream::Ok;
}


bool ShapeStorage::saveToFile(
    const QString& fileName
) const
{
    QFile file(fileName);

    if (!file.open(
            QIODevice::WriteOnly |
            QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    return saveToStream(out);
}
QString ShapeStorage::createSnapshot() const
{
    QString data;

    QTextStream out(
        &data,
        QIODevice::WriteOnly
    );

    if (!saveToStream(out))
    {
        return QString();
    }

    int selectedCount = 0;

    for (Shape* shape : shapes)
    {
        if (shape->isSelected())
        {
            ++selectedCount;
        }
    }

    out << "SELECTION "
        << selectedCount
        << "\n";

    for (int i = 0;
         i < static_cast<int>(shapes.size());
         ++i)
    {
        if (shapes[i]->isSelected())
        {
            out << i << " ";
        }
    }

    out << "\n";

    return out.status() == QTextStream::Ok
        ? data
        : QString();
}
bool ShapeStorage::loadFromStream(
    QTextStream& in,
    const ShapeFactory& factory,
    const QRect& bounds,
    bool notifyAfterLoad
)
{
    int objectCount;
    in >> objectCount;

    if (in.status() != QTextStream::Ok ||
        objectCount < 0)
    {
        return false;
    }

    std::vector<Shape*> loadedShapes;
    std::vector<Arrow*> loadedArrows;

    auto clearLoadedShapes =
        [&loadedShapes]()
        {
            for (Shape* shape : loadedShapes)
            {
                delete shape;
            }

            loadedShapes.clear();
        };

    auto clearLoadedData =
        [&loadedShapes, &loadedArrows]()
        {
            for (Arrow* arrow : loadedArrows)
            {
                delete arrow;
            }

            loadedArrows.clear();

            for (Shape* shape : loadedShapes)
            {
                delete shape;
            }

            loadedShapes.clear();
        };

    for (int i = 0; i < objectCount; ++i)
    {
        QString type;
        in >> type;

        if (in.status() != QTextStream::Ok)
        {
            clearLoadedShapes();
            return false;
        }

        Shape* shape =
            factory.create(type);

        if (shape == nullptr)
        {
            clearLoadedShapes();
            return false;
        }

        if (!shape->load(in, factory) ||
            !shape->fitsInBounds(bounds))
        {
            delete shape;
            clearLoadedShapes();
            return false;
        }

        loadedShapes.push_back(shape);
    }

    QString marker;
    int arrowCount;

    in >> marker >> arrowCount;

    if (in.status() != QTextStream::Ok ||
        marker != "ARROWS" ||
        arrowCount < 0)
    {
        clearLoadedShapes();
        return false;
    }

    for (int i = 0; i < arrowCount; ++i)
    {
        QString sourcePath;
        QString targetPath;

        in >> sourcePath
           >> targetPath;

        if (in.status() != QTextStream::Ok)
        {
            clearLoadedData();
            return false;
        }

        Shape* source =
            resolvePath(
                loadedShapes,
                sourcePath
            );

        Shape* target =
            resolvePath(
                loadedShapes,
                targetPath
            );

        if (source == nullptr ||
            target == nullptr ||
            source == target)
        {
            clearLoadedData();
            return false;
        }

        loadedArrows.push_back(
            new Arrow(
                source,
                target
            )
        );
    }

    for (Arrow* arrow : arrows)
    {
        delete arrow;
    }

    arrows.clear();

    for (Shape* shape : shapes)
    {
        delete shape;
    }

    shapes.clear();

    shapes.swap(loadedShapes);
    arrows.swap(loadedArrows);

    if (notifyAfterLoad)
    {
        notifyObservers();
    }

    return true;
}

bool ShapeStorage::loadFromFile(
    const QString& fileName,
    const ShapeFactory& factory,
    const QRect& bounds
)
{
    QFile file(fileName);

    if (!file.open(
            QIODevice::ReadOnly |
            QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);

    return loadFromStream(
        in,
        factory,
        bounds
    );
}


bool ShapeStorage::restoreSnapshot(
    const QString& data,
    const ShapeFactory& factory,
    const QRect& bounds
)
{
    QString copy = data;

    QTextStream in(
        &copy,
        QIODevice::ReadOnly
    );

    if (!loadFromStream(
            in,
            factory,
            bounds,
            false))
    {
        return false;
    }

    QString marker;
    int selectedCount;

    in >> marker >> selectedCount;

    if (in.status() != QTextStream::Ok ||
        marker != "SELECTION" ||
        selectedCount < 0)
    {
        notifyObservers();
        return false;
    }

    for (int i = 0; i < selectedCount; ++i)
    {
        int index;
        in >> index;

        if (in.status() != QTextStream::Ok ||
            index < 0 ||
            index >= static_cast<int>(shapes.size()))
        {
            notifyObservers();
            return false;
        }

        shapes[index]->setSelected(true);
    }

    notifyObservers();

    return true;
}
void ShapeStorage::subscribe(
    StorageObserver* observer
)
{
    if (observer != nullptr &&
        std::find(
            observers.begin(),
            observers.end(),
            observer
        ) == observers.end())
    {
        observers.push_back(observer);
    }
}


void ShapeStorage::unsubscribe(
    StorageObserver* observer
)
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
void ShapeStorage::addArrow(
    Arrow* arrow
)
{
    if (arrow != nullptr)
    {
        arrows.push_back(arrow);
    }
}


int ShapeStorage::getArrowCount() const
{
    return static_cast<int>(
        arrows.size()
    );
}


Arrow* ShapeStorage::getArrow(
    int index
) const
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

    for (Shape* child :
         group->getChildren())
    {
        if (containsShape(
                child,
                searched))
        {
            return true;
        }
    }

    return false;
}
void ShapeStorage::removeArrowsReferencing(
    Shape* shape
)
{
    size_t i = 0;

    while (i < arrows.size())
    {
        Shape* source =
            arrows[i]->getSource();

        Shape* target =
            arrows[i]->getTarget();

        bool sourceInside =
            containsShape(
                shape,
                source
            );

        bool targetInside =
            containsShape(
                shape,
                target
            );

        if (sourceInside ||
            targetInside)
        {
            delete arrows[i];

            arrows.erase(
                arrows.begin() + i
            );
        }
        else
        {
            ++i;
        }
    }
}
void ShapeStorage::removeArrowsDirectlyReferencing(
    Shape* shape
)
{
    size_t i = 0;

    while (i < arrows.size())
    {
        if (arrows[i]->getSource() == shape ||
            arrows[i]->getTarget() == shape)
        {
            delete arrows[i];

            arrows.erase(
                arrows.begin() + i
            );
        }
        else
        {
            ++i;
        }
    }
}
QString ShapeStorage::findPath(
    Shape* target
) const
{
    for (int i = 0;
         i < static_cast<int>(shapes.size());
         ++i)
    {
        QString result;

        if (findPathRecursive(
                shapes[i],
                target,
                QString::number(i),
                result))
        {
            return result;
        }
    }

    return QString();
}


bool ShapeStorage::findPathRecursive(
    Shape* current,
    Shape* target,
    const QString& currentPath,
    QString& result
) const
{
    if (current == target)
    {
        result = currentPath;
        return true;
    }

    Group* group =
        dynamic_cast<Group*>(current);

    if (group == nullptr)
    {
        return false;
    }

    const std::vector<Shape*>& children =
        group->getChildren();

    for (int i = 0;
         i < static_cast<int>(children.size());
         ++i)
    {
        QString childPath =
            currentPath +
            "/" +
            QString::number(i);

        if (findPathRecursive(
                children[i],
                target,
                childPath,
                result))
        {
            return true;
        }
    }

    return false;
}


Shape* ShapeStorage::resolvePath(
    const std::vector<Shape*>& rootShapes,
    const QString& path
) const
{
    QStringList parts =
        path.split('/');

    if (parts.empty())
    {
        return nullptr;
    }

    bool ok = false;

    int rootIndex =
        parts[0].toInt(&ok);

    if (!ok ||
        rootIndex < 0 ||
        rootIndex >=
            static_cast<int>(rootShapes.size()))
    {
        return nullptr;
    }

    Shape* current =
        rootShapes[rootIndex];


    for (int i = 1;
         i < parts.size();
         ++i)
    {
        Group* group =
            dynamic_cast<Group*>(current);

        if (group == nullptr)
        {
            return nullptr;
        }

        int childIndex =
            parts[i].toInt(&ok);

        const std::vector<Shape*>& children =
            group->getChildren();

        if (!ok ||
            childIndex < 0 ||
            childIndex >=
                static_cast<int>(children.size()))
        {
            return nullptr;
        }

        current =
            children[childIndex];
    }

    return current;
}