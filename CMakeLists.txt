cmake_minimum_required(VERSION 3.16)

project(gomoku-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(
    gomoku-qt

    main.cpp

    frontend/src/main-window.cpp
    frontend/src/board-widget.cpp
    frontend/include/main-window.h
    frontend/include/board-widget.h

    backend/src/game-logic.cpp
    backend/src/alpha-beta-ai.cpp
    backend/include/game-logic.h
    backend/include/alpha-beta-ai.h

)
target_link_libraries(gomoku-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(gomoku-qt PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS gomoku-qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
