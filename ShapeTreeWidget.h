#ifndef OOP7_SHAPETREEWIDGET_H
#define OOP7_SHAPETREEWIDGET_H

#include <QTreeWidget>
#include <unordered_map>
#include "StorageObserver.h"

class Shape;
class ShapeStorage;

class ShapeTreeWidget :
    public QTreeWidget,
    public StorageObserver
{
    Q_OBJECT
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
    std::unordered_map<QTreeWidgetItem*, ::Shape*> itemToOwner;
    std::unordered_map<::Shape*, QTreeWidgetItem*> rootItems;

    void onTreeSelectionChanged();

    void addShapeNode(
        QTreeWidgetItem* parent,
        ::Shape* shape,
        ::Shape* owner
    );
    signals:
    void shapeSelectionChanged();
};

#endif