#ifndef OOP7_SHAPETREEWIDGET_H
#define OOP7_SHAPETREEWIDGET_H

#include <QTreeWidget>

#include "StorageObserver.h"

class Shape;
class ShapeStorage;

class ShapeTreeWidget :
    public QTreeWidget,
    public StorageObserver
{
public:
    explicit ShapeTreeWidget(
        ShapeStorage* storage,
        QWidget* parent = nullptr
    );

    ~ShapeTreeWidget() override;

    void onStorageChanged() override;

private:
    ShapeStorage* storage;

    void rebuildTree();

    void addShapeNode(
    QTreeWidgetItem* parent,
    ::Shape* shape);
};

#endif