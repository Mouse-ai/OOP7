#include "ShapeTreeWidget.h"

#include "ShapeStorage.h"
#include "Shape.h"
#include "Group.h"

ShapeTreeWidget::ShapeTreeWidget(
    ShapeStorage* storage,
    QWidget* parent
)
    : QTreeWidget(parent),
      storage(storage)
{
    setHeaderLabel("Объекты");

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
    clear();

    if (storage == nullptr)
    {
        return;
    }

    for (int i = 0; i < storage->getCount(); ++i)
    {
        ::Shape* shape = storage->getObject(i);

        addShapeNode(
            invisibleRootItem(),
            shape
        );
    }

    expandAll();
}
void ShapeTreeWidget::addShapeNode(
    QTreeWidgetItem* parent,
    ::Shape* shape
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

    Group* group =
        dynamic_cast<Group*>(shape);

    if (group == nullptr)
    {
        return;
    }

    for (::Shape* child : group->getChildren())
    {
        addShapeNode(item, child);
    }
}