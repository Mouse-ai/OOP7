#include "ShapeTreeWidget.h"

#include "ShapeStorage.h"
#include "Shape.h"
#include "Group.h"
#include <QSignalBlocker>

ShapeTreeWidget::ShapeTreeWidget(
    ShapeStorage* storage,
    QWidget* parent
)
    : QTreeWidget(parent),
      storage(storage)
{
    setHeaderLabel("Объекты");
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(
        this,
        &QTreeWidget::itemSelectionChanged,
        this,
        &ShapeTreeWidget::onTreeSelectionChanged
    );
    if (storage != nullptr)
    {
        storage->subscribe(this);
    }

    rebuildTree();
}

ShapeTreeWidget::~ShapeTreeWidget()
{
    if (storage != nullptr)
    {
        storage->unsubscribe(this);
    }
}
void ShapeTreeWidget::onStorageChanged()
{
    rebuildTree();
}
void ShapeTreeWidget::rebuildTree()
{
    QSignalBlocker blocker(this);

    clear();

    itemToOwner.clear();
    rootItems.clear();

    if (storage == nullptr)
    {
        return;
    }

    for (int i = 0; i < storage->getCount(); ++i)
    {
        ::Shape* shape = storage->getObject(i);

        addShapeNode(
            invisibleRootItem(),
            shape,
            shape
        );

        if (shape->isSelected())
        {
            rootItems[shape]->setSelected(true);
        }
    }

    expandAll();
}
void ShapeTreeWidget::addShapeNode(
    QTreeWidgetItem* parent,
    ::Shape* shape,
    ::Shape* owner
)
{
    if (shape == nullptr)
    {
        return;
    }

    QTreeWidgetItem* item =
        new QTreeWidgetItem(parent);

    item->setText(
        0,
        shape->typeName()
    );
    itemToOwner[item] = owner;

    if (shape == owner)
    {
        rootItems[owner] = item;
    }

    Group* group =
        dynamic_cast<Group*>(shape);

    if (group == nullptr)
    {
        return;
    }

    for (::Shape* child : group->getChildren())
    {
        addShapeNode(item, child,owner);
    }
}
void ShapeTreeWidget::onTreeSelectionChanged()
{
    if (storage == nullptr)
    {
        return;
    }

    storage->clearSelection();

    for (QTreeWidgetItem* item : selectedItems())
    {
        auto it = itemToOwner.find(item);

        if (it != itemToOwner.end())
        {
            it->second->setSelected(true);
        }
    }

    storage->notifyObservers();

    emit shapeSelectionChanged();
}