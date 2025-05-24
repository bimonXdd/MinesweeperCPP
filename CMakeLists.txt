cmake_minimum_required(VERSION 3.16)
project(Minesweeper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Minesweeper
        main.cpp
        MainWindow.cpp
        MainWindow.h
        grid.cpp
        grid.h
        mainwindow.ui
)

target_link_libraries(Minesweeper PRIVATE Qt6::Widgets)