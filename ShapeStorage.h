//
// Created by alexei on 25.10.2026.
//

#ifndef OOP4_SHAPESTORAGE_H
#define OOP4_SHAPESTORAGE_H
#include <vector>
#include "Shape.h"
#include "StorageObserver.h"
class ShapeStorage {
private:
    std::vector<Shape *> shapes;
    std::vector<StorageObserver*> observers;

public:
    void add(Shape *shape);

    int getCount() const;
    Shape *getObject(int index) const;

    void clearSelection();
    void removeSelected();
    std::vector<Shape*> takeSelected();
    bool saveToFile(const QString& fileName) const;
    bool loadFromFile(
        const QString& fileName,
        const ShapeFactory& factory,
        const QRect& bounds
    );
    ~ShapeStorage();
    void subscribe(StorageObserver* observer);
    void unsubscribe(StorageObserver* observer);
    void notifyObservers();
};


#endif //OOP4_SHAPESTORAGE_H
