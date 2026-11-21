#include "MainWindow.h"
#include "Ccircle.h"
#include "CRectangle.h"
#include "CSquare.h"
#include "CTriangle.h"
#include <QColorDialog>
#include <QActionGroup>
#include "CEllipse.h"
#include "CLine.h"
#include "Group.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include "ShapeTreeWidget.h"
#include "ShapeFactory.h"
#include "Arrow.h"
#include <QStatusBar>
#include "ActionCommand.h"
#include "SnapshotCommand.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setupActions();

    QWidget *workArea = new QWidget(this);
    workArea->setAttribute(Qt::WA_TransparentForMouseEvents);

    setCentralWidget(workArea);

    QDockWidget* treeDock =
        new QDockWidget("Дерево объектов", this);

    shapeTree =
        new ShapeTreeWidget(&storage, treeDock);

    connect(
        shapeTree,
        &ShapeTreeWidget::shapeSelectionChanged,
        this,
        [this]()
        {
            update();
        }
    );

    treeDock->setWidget(shapeTree);

    addDockWidget(
        Qt::RightDockWidgetArea,
        treeDock
    );
}

void MainWindow::executeUndoable(
    const std::function<void()>& action
)
{
    commandHistory.execute(
        std::make_unique<ActionCommand>(
            storage,
            undoFactory,
            getWorkAreaRect(),
            action
        )
    );

    undoAction->setEnabled(
        !commandHistory.empty()
    );
}

void MainWindow::recordCommand(
    const QString& beforeState
)
{
    QString afterState =
        storage.createSnapshot();

    if (beforeState.isEmpty() ||
        afterState.isEmpty() ||
        beforeState == afterState)
    {
        return;
    }

    commandHistory.execute(
        std::make_unique<SnapshotCommand>(
            storage,
            undoFactory,
            getWorkAreaRect(),
            beforeState,
            afterState
        )
    );

    undoAction->setEnabled(
        !commandHistory.empty()
    );
}

void MainWindow::undoLastCommand()
{
    creatingArrow = false;
    arrowSource = nullptr;

    statusBar()->clearMessage();

    if (!commandHistory.undo())
    {
        return;
    }

    undoAction->setEnabled(
        !commandHistory.empty()
    );

    update();
}

void MainWindow::changeSelectedColor()
{
    QColor newColor = QColorDialog::getColor();

    if (!newColor.isValid())
    {
        return;
    }

    executeUndoable(
        [this, newColor]()
        {
            for (int i = 0; i < storage.getCount(); ++i)
            {
                Shape *shape = storage.getObject(i);

                if (shape->isSelected())
                {
                    shape->setColor(newColor);
                }
            }

            storage.clearSelection();
            storage.notifyObservers();
        }
    );

    update();
}

QRect MainWindow::getWorkAreaRect() const
{
    return centralWidget()->geometry();
}

void MainWindow::setupActions()
{
    circleAction = new QAction("Круг", this);
    rectangleAction = new QAction("Прямоугольник", this);
    colorAction = new QAction("Цвет", this);
    squareAction = new QAction("Квадрат", this);
    triangleAction = new QAction("Треугольник", this);
    ellipseAction = new QAction("Эллипс", this);
    lineAction = new QAction("Отрезок", this);
    groupAction = new QAction("Сгруппировать", this);
    saveAction = new QAction("Сохранить", this);
    loadAction = new QAction("Загрузить", this);
    arrowAction = new QAction("Стрелка A → B", this);
    undoAction = new QAction("Отменить", this);

    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setEnabled(false);

    connect(
        undoAction,
        &QAction::triggered,
        this,
        [this]()
        {
            undoLastCommand();
        }
    );

    connect(
        arrowAction,
        &QAction::triggered,
        this,
        [this]()
        {
            creatingArrow = true;
            arrowSource = nullptr;

            storage.clearSelection();
            storage.notifyObservers();

            statusBar()->showMessage(
                "Выберите источник стрелки"
            );

            update();
        }
    );

    connect(
        saveAction,
        &QAction::triggered,
        this,
        [this]()
        {
            saveProject();
        }
    );

    connect(
        loadAction,
        &QAction::triggered,
        this,
        [this]()
        {
            loadProject();
        }
    );

    connect(
        groupAction,
        &QAction::triggered,
        this,
        [this]()
        {
            groupSelected();
        }
    );

    connect(
        triangleAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Triangle;
        }
    );

    connect(
        squareAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Square;
        }
    );

    connect(
        colorAction,
        &QAction::triggered,
        this,
        [this]()
        {
            changeSelectedColor();
        }
    );

    connect(
        circleAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Circle;
        }
    );

    connect(
        rectangleAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Rectangle;
        }
    );

    connect(
        ellipseAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Ellipse;
        }
    );

    connect(
        lineAction,
        &QAction::triggered,
        this,
        [this]()
        {
            currentShapeType = ShapeType::Line;
        }
    );

    QMenu* fileMenu = menuBar()->addMenu("Файл");

    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);

    QMenu *shapeMenu = menuBar()->addMenu("Фигура");

    shapeMenu->addAction(circleAction);
    shapeMenu->addAction(rectangleAction);
    shapeMenu->addAction(squareAction);
    shapeMenu->addAction(triangleAction);
    shapeMenu->addAction(ellipseAction);
    shapeMenu->addAction(lineAction);

    QMenu *editMenu = menuBar()->addMenu("Редактирование");

    editMenu->addAction(undoAction);
    editMenu->addSeparator();
    editMenu->addAction(colorAction);
    editMenu->addAction(groupAction);
    editMenu->addAction(arrowAction);

    QToolBar *toolBar = addToolBar("Фигуры");

    toolBar->addAction(circleAction);
    toolBar->addAction(rectangleAction);
    toolBar->addAction(squareAction);
    toolBar->addAction(triangleAction);

    groupAction->setShortcut(QKeySequence("Ctrl+G"));

    toolBar->addAction(ellipseAction);
    toolBar->addAction(lineAction);
    toolBar->addAction(arrowAction);
    toolBar->addAction(colorAction);

    circleAction->setCheckable(true);
    rectangleAction->setCheckable(true);
    squareAction->setCheckable(true);
    triangleAction->setCheckable(true);
    ellipseAction->setCheckable(true);
    lineAction->setCheckable(true);

    QActionGroup *shapeGroup = new QActionGroup(this);

    shapeGroup->addAction(circleAction);
    shapeGroup->addAction(rectangleAction);
    shapeGroup->addAction(squareAction);
    shapeGroup->addAction(triangleAction);
    shapeGroup->addAction(ellipseAction);
    shapeGroup->addAction(lineAction);

    shapeGroup->setExclusive(true);

    circleAction->setChecked(true);

    ungroupAction = new QAction("Разгруппировать", this);

    connect(
        ungroupAction,
        &QAction::triggered,
        this,
        [this]()
        {
            ungroupSelected();
        }
    );

    ungroupAction->setShortcut(
        QKeySequence("Ctrl+Shift+G")
    );

    saveAction->setShortcut(QKeySequence::Save);
    loadAction->setShortcut(QKeySequence::Open);

    editMenu->addAction(ungroupAction);
}

void MainWindow::ungroupSelected()
{
    bool hasSelectedGroup = false;

    for (int i = 0; i < storage.getCount(); ++i)
    {
        Shape* shape = storage.getObject(i);

        if (shape->isSelected() &&
            dynamic_cast<Group*>(shape) != nullptr)
        {
            hasSelectedGroup = true;
            break;
        }
    }

    if (!hasSelectedGroup)
    {
        return;
    }

    executeUndoable(
        [this]()
        {
            for (int i = 0; i < storage.getCount(); ++i)
            {
                Shape* shape = storage.getObject(i);

                if (shape->isSelected() &&
                    dynamic_cast<Group*>(shape) == nullptr)
                {
                    shape->setSelected(false);
                }
            }

            std::vector<Shape*> selectedGroups =
                storage.takeSelected();

            for (Shape* shape : selectedGroups)
            {
                Group* group =
                    dynamic_cast<Group*>(shape);

                storage.removeArrowsDirectlyReferencing(group);

                std::vector<Shape*> children =
                    group->takeChildren();

                for (Shape* child : children)
                {
                    child->setSelected(true);
                    storage.add(child);
                }

                delete group;
            }
        }
    );

    update();
}

void MainWindow::groupSelected()
{
    int selectedCount = 0;

    for (int i = 0; i < storage.getCount(); ++i)
    {
        if (storage.getObject(i)->isSelected())
        {
            ++selectedCount;
        }
    }

    if (selectedCount < 2)
    {
        return;
    }

    executeUndoable(
        [this]()
        {
            std::vector<Shape*> selectedShapes =
                storage.takeSelected();

            Group* group = new Group();

            for (Shape* shape : selectedShapes)
            {
                group->add(shape);
            }

            group->setSelected(true);
            storage.add(group);
        }
    );

    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    QPoint point = event->pos();

    if (creatingArrow)
    {
        Shape* clickedShape =
            findShapeAt(point);

        if (clickedShape == nullptr)
        {
            return;
        }

        if (arrowSource == nullptr)
        {
            arrowSource = clickedShape;

            storage.clearSelection();
            arrowSource->setSelected(true);
            storage.notifyObservers();

            statusBar()->showMessage(
                "Теперь выберите цель стрелки"
            );

            update();
            return;
        }

        if (clickedShape == arrowSource)
        {
            statusBar()->showMessage(
                "Источник и цель должны быть разными"
            );

            return;
        }

        Shape* source = arrowSource;

        executeUndoable(
            [this, source, clickedShape]()
            {
                storage.addArrow(
                    new Arrow(
                        source,
                        clickedShape
                    )
                );

                storage.clearSelection();
                storage.notifyObservers();
            }
        );

        creatingArrow = false;
        arrowSource = nullptr;

        statusBar()->clearMessage();

        update();
        return;
    }

    bool ctrlPressed =
        event->modifiers() & Qt::ControlModifier;

    bool shapeFound = false;

    for (int i = storage.getCount() - 1; i >= 0; --i)
    {
        Shape *shape = storage.getObject(i);

        if (shape->contains(point.x(), point.y()))
        {
            shapeFound = true;

            if (ctrlPressed)
            {
                shape->toggleSelected();
            }
            else
            {
                storage.clearSelection();
                shape->setSelected(true);
            }

            dragging = shape->isSelected();

            if (dragging)
            {
                lastMousePosition = point;
                dragStartSnapshot =
                    storage.createSnapshot();
            }

            storage.notifyObservers();
            break;
        }
    }

    if (!shapeFound)
    {
        executeUndoable(
            [this, point, ctrlPressed]()
            {
                if (!ctrlPressed)
                {
                    storage.clearSelection();
                }

                Shape *newShape = nullptr;

                if (currentShapeType == ShapeType::Circle)
                {
                    newShape = new CCircle(point.x(), point.y());
                }
                else if (currentShapeType == ShapeType::Rectangle)
                {
                    newShape = new CRectangle(point.x(), point.y());
                }
                else if (currentShapeType == ShapeType::Square)
                {
                    newShape = new CSquare(point.x(), point.y());
                }
                else if (currentShapeType == ShapeType::Triangle)
                {
                    newShape = new CTriangle(point.x(), point.y());
                }
                else if (currentShapeType == ShapeType::Ellipse)
                {
                    newShape = new CEllipse(point.x(), point.y());
                }
                else if (currentShapeType == ShapeType::Line)
                {
                    newShape = new CLine(point.x(), point.y());
                }

                if (newShape == nullptr)
                {
                    return;
                }

                if (!newShape->fitsInBounds(getWorkAreaRect()))
                {
                    delete newShape;
                    return;
                }

                if (ctrlPressed)
                {
                    newShape->setSelected(true);
                }

                storage.add(newShape);
            }
        );
    }

    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (!dragging ||
        !(event->buttons() & Qt::LeftButton))
    {
        return;
    }

    QPoint currentPosition =
        event->position().toPoint();

    int dx =
        currentPosition.x() -
        lastMousePosition.x();

    int dy =
        currentPosition.y() -
        lastMousePosition.y();

    if (dx == 0 && dy == 0)
    {
        return;
    }

    QRect bounds = getWorkAreaRect();

    MoveContext context;

    for (int i = 0; i < storage.getCount(); ++i)
    {
        Shape* shape = storage.getObject(i);

        if (shape->isSelected())
        {
            shape->moveWithContext(
                dx,
                dy,
                bounds,
                context
            );
        }
    }

    lastMousePosition =
        currentPosition;

    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (dragging &&
            !dragStartSnapshot.isEmpty())
        {
            recordCommand(
                dragStartSnapshot
            );

            dragStartSnapshot.clear();
        }

        dragging = false;
    }

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);

    QPainter painter(this);

    for (int i = 0; i < storage.getCount(); ++i)
    {
        storage.getObject(i)->draw(painter);
    }

    for (int i = 0; i < storage.getArrowCount(); ++i)
    {
        storage.getArrow(i)->draw(painter);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        executeUndoable(
            [this]()
            {
                storage.removeSelected();
            }
        );

        update();
        return;
    }

    if (event->key() == Qt::Key_1)
    {
        currentShapeType = ShapeType::Circle;
        circleAction->setChecked(true);
        return;
    }

    if (event->key() == Qt::Key_2)
    {
        currentShapeType = ShapeType::Rectangle;
        rectangleAction->setChecked(true);
        return;
    }

    if (event->key() == Qt::Key_3)
    {
        currentShapeType = ShapeType::Square;
        squareAction->setChecked(true);
        return;
    }

    if (event->key() == Qt::Key_4)
    {
        currentShapeType = ShapeType::Triangle;
        triangleAction->setChecked(true);
        return;
    }

    if (event->key() == Qt::Key_5)
    {
        currentShapeType = ShapeType::Ellipse;
        ellipseAction->setChecked(true);
        return;
    }

    if (event->key() == Qt::Key_6)
    {
        currentShapeType = ShapeType::Line;
        lineAction->setChecked(true);
        return;
    }

    int dx = 0;
    int dy = 0;

    if (event->key() == Qt::Key_Left)
    {
        dx = -5;
    }
    else if (event->key() == Qt::Key_Right)
    {
        dx = 5;
    }
    else if (event->key() == Qt::Key_Up)
    {
        dy = -5;
    }
    else if (event->key() == Qt::Key_Down)
    {
        dy = 5;
    }

    if (dx != 0 || dy != 0)
    {
        executeUndoable(
            [this, dx, dy]()
            {
                QRect bounds =
                    getWorkAreaRect();

                MoveContext context;

                for (int i = 0;
                     i < storage.getCount();
                     ++i)
                {
                    Shape *shape =
                        storage.getObject(i);

                    if (shape->isSelected())
                    {
                        shape->moveWithContext(
                            dx,
                            dy,
                            bounds,
                            context
                        );
                    }
                }
            }
        );

        update();
        return;
    }

    if (event->key() == Qt::Key_C)
    {
        changeSelectedColor();
        return;
    }

    int resizeDelta = 0;

    if (event->key() == Qt::Key_Plus)
    {
        resizeDelta = 5;
    }
    else if (event->key() == Qt::Key_Minus)
    {
        resizeDelta = -5;
    }

    if (resizeDelta != 0)
    {
        executeUndoable(
            [this, resizeDelta]()
            {
                QRect bounds =
                    getWorkAreaRect();

                for (int i = 0;
                     i < storage.getCount();
                     ++i)
                {
                    Shape *shape =
                        storage.getObject(i);

                    if (shape->isSelected())
                    {
                        shape->resize(
                            resizeDelta,
                            bounds
                        );
                    }
                }
            }
        );

        update();
        return;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    update();
}

void MainWindow::saveProject()
{
    QString fileName =
        QFileDialog::getSaveFileName(
            this,
            "Сохранить проект",
            QString(),
            "Текстовые файлы (*.txt)"
        );

    if (fileName.isEmpty())
    {
        return;
    }

    if (!fileName.endsWith(".txt"))
    {
        fileName += ".txt";
    }

    if (!storage.saveToFile(fileName))
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            "Не удалось сохранить проект."
        );
    }
}

void MainWindow::loadProject()
{
    QString fileName =
        QFileDialog::getOpenFileName(
            this,
            "Загрузить проект",
            QString(),
            "Текстовые файлы (*.txt)"
        );

    if (fileName.isEmpty())
    {
        return;
    }

    bool loaded = false;

    executeUndoable(
        [this, &loaded, fileName]()
        {
            loaded = storage.loadFromFile(
                fileName,
                undoFactory,
                getWorkAreaRect()
            );
        }
    );

    if (!loaded)
    {
        QMessageBox::critical(
            this,
            "Ошибка",
            "Не удалось загрузить проект. "
            "Файл повреждён или содержит некорректные данные."
        );

        return;
    }

    update();
}

Shape* MainWindow::findShapeAt(
    const QPoint& point
) const
{
    for (int i = storage.getCount() - 1;
         i >= 0;
         --i)
    {
        Shape* shape =
            storage.getObject(i);

        if (shape->contains(
                point.x(),
                point.y()))
        {
            return shape;
        }
    }

    return nullptr;
}
