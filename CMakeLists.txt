cmake_minimum_required(VERSION 4.2)
project(OOP7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(OOP7
        main.cpp

        MainWindow.cpp
        MainWindow.h

        Shape.cpp
        Shape.h

        ShapeStorage.cpp
        ShapeStorage.h

        Ccircle.cpp
        Ccircle.h

        CRectangle.cpp
        CRectangle.h

        CSquare.cpp
        CSquare.h

        CEllipse.cpp
        CEllipse.h

        CLine.cpp
        CLine.h

        CTriangle.cpp
        CTriangle.h
        Group.cpp
        Group.h
        ShapeFactory.cpp
        ShapeFactory.h
        StorageObserver.cpp
        StorageObserver.h
        ShapeTreeWidget.cpp
        ShapeTreeWidget.h
        ShapeMoveObserver.cpp
        ShapeMoveObserver.h
        Arrow.cpp
        Arrow.h
        Command.h
        SnapshotCommand.cpp
        SnapshotCommand.h
        CommandHistory.cpp
        CommandHistory.h
)
target_link_libraries(OOP7
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

