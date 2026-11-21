#include "Group.h"
#include "ShapeFactory.h"

Group::Group()
    : Shape(0, 0)
{
}

Group::~Group()
{
    for (Shape* shape : children)
    {
        delete shape;
    }
}

void Group::add(Shape* shape)
{
    if (shape != nullptr)
    {
        shape->setSelected(false);
        children.push_back(shape);
    }
}
QRect Group::boundingRect() const
{
    if (children.empty())
    {
        return QRect();
    }

    QRect result = children[0]->boundingRect();

    for (int i = 1; i < children.size(); ++i)
    {
        result = result.united(children[i]->boundingRect());
    }

    return result;
}
void Group::draw(QPainter& painter) const
{
    for (Shape* shape : children)
    {
        shape->draw(painter);
    }

    if (selected)
    {
        painter.save();

        QPen pen(Qt::red);
        pen.setWidth(2);
        pen.setStyle(Qt::DashLine);

        painter.setPen(pen);
        painter.drawRect(boundingRect());

        painter.restore();
    }
}
bool Group::contains(int pointX, int pointY) const
{
    return boundingRect().contains(pointX, pointY);
}
void Group::move(int dx, int dy, const QRect& bounds)
{
    QRect newRect = boundingRect().translated(dx, dy);

    if (!bounds.contains(newRect))
    {
        return;
    }

    for (Shape* shape : children)
    {
        shape->move(dx, dy, bounds);
    }
}
bool Group::canResize(int delta) const
{
    return false;
}

void Group::changeSize(int delta)
{
}
std::vector<Shape*> Group::takeChildren()
{
    std::vector<Shape*> result = children;
    children.clear();

    return result;
}
void Group::save(QTextStream& out, int indent) const
{
    QString spaces(indent, ' ');

    out << spaces
        << "GROUP "
        << children.size()
        << "\n";

    for (Shape* shape : children)
    {
        shape->save(out, indent + 2);
    }
}
bool Group::load(
    QTextStream& in,
    const ShapeFactory& factory
)
{
    int childCount;

    in >> childCount;

    if (in.status() != QTextStream::Ok ||
        childCount < 2)
    {
        return false;
    }

    std::vector<Shape*> loadedChildren;

    for (int i = 0; i < childCount; ++i)
    {
        QString type;

        in >> type;

        if (in.status() != QTextStream::Ok)
        {
            for (Shape* shape : loadedChildren)
            {
                delete shape;
            }

            return false;
        }

        Shape* child = factory.create(type);

        if (child == nullptr)
        {
            for (Shape* shape : loadedChildren)
            {
                delete shape;
            }

            return false;
        }

        if (!child->load(in, factory))
        {
            delete child;

            for (Shape* shape : loadedChildren)
            {
                delete shape;
            }

            return false;
        }

        loadedChildren.push_back(child);
    }

    for (Shape* shape : children)
    {
        delete shape;
    }

    children.clear();

    for (Shape* shape : loadedChildren)
    {
        add(shape);
    }

    selected = false;

    return true;
}
void Group::setColor(const QColor& newColor)
{
    Shape::setColor(newColor);

    for (Shape* shape : children)
    {
        shape->setColor(newColor);
    }
}
bool Group::resize(int delta, const QRect& bounds)
{
    std::vector<Shape*> resizedShapes;

    for (Shape* shape : children)
    {
        if (!shape->resize(delta, bounds))
        {
            for (Shape* resizedShape : resizedShapes)
            {
                resizedShape->resize(-delta, bounds);
            }

            return false;
        }

        resizedShapes.push_back(shape);
    }

    return true;
}
const std::vector<Shape*>& Group::getChildren() const
{
    return children;
}