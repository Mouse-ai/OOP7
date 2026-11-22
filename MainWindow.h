#ifndef OOP7_MAINWINDOW_H
#define OOP7_MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include "ShapeStorage.h"
#include <QRect>
#include <QResizeEvent>
#include "CommandHistory.h"
#include "ShapeFactory.h"

#include <functional>

class ShapeTreeWidget;

class MainWindow : public QMainWindow
{
public:
    enum class ShapeType
    {
        Circle,
        Rectangle,
        Square,
        Triangle,
        Ellipse,
        Line
    };

    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool dragging = false;
    QPoint lastMousePosition;

    void setupActions();
    QRect getWorkAreaRect() const;
    void changeSelectedColor();

    QAction *circleAction;
    QAction *rectangleAction;
    QAction *colorAction;
    QAction *triangleAction;
    QAction *squareAction;
    QAction *ellipseAction;
    QAction *lineAction;
    QAction *groupAction;
    QAction* ungroupAction;
    QAction* saveAction;
    QAction* loadAction;
    QAction* arrowAction;
    QAction* undoAction;

    void saveProject();
    void loadProject();
    void ungroupSelected();
    void groupSelected();

    ShapeStorage storage;
    ShapeType currentShapeType = ShapeType::Circle;
    ShapeTreeWidget* shapeTree;

    bool creatingArrow = false;
    Shape* arrowSource = nullptr;
    Shape* findShapeAt(const QPoint& point) const;

    CommandHistory commandHistory;
    DefaultShapeFactory undoFactory;
    QString dragStartSnapshot;

    void executeUndoable(
        const std::function<void()>& action
    );

    void recordCommand(
        const QString& beforeState
    );

    void undoLastCommand();
};

#endif
