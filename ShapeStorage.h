//
// Created by alexei on 25.10.2026.
//

#ifndef OOP4_SHAPESTORAGE_H
#define OOP4_SHAPESTORAGE_H
#include <vector>
#include "Shape.h"
#include "StorageObserver.h"

class Arrow;
class ShapeStorage {
private:
    std::vector<Shape *> shapes;
    std::vector<StorageObserver*> observers;
    std::vector<Arrow*> arrows;
    bool containsShape(Shape* root,Shape* searched) const;
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
    void addArrow(Arrow* arrow);

    int getArrowCount() const;
    Arrow* getArrow(int index) const;
    void removeArrowsReferencing(Shape* shape);
};


#endif //OOP4_SHAPESTORAGE_H
