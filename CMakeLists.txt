cmake_minimum_required(VERSION 3.14)

project(BattleShip VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick QuickControls2)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick QuickControls2)

set(PROJECT_SOURCES
        src/main.cpp
        src/cell.cpp src/cell.h
        src/connector.cpp src/connector.h
        src/field.cpp src/field.h
        src/game.cpp src/game.h
        qml.qrc
        fonts.qrc
        images.qrc
)

# Resources:
set(qml_resource_files
    "QML/Cell.qml"
    "QML/Field.qml"
    "QML/FieldInitializer.qml"
    "QML/Game.qml"
    "QML/HomePage.qml"
    "QML/NamesInput.qml"
    "QML/OpponentSelectPage.qml"
    "QML/Restart.qml"
    "QML/ShipCounter.qml"
    "QML/ShipCounterRow.qml"
    "QML/WaitingPage.qml"
    "data/Quando-Regular.ttf"
    "data/ship1.png"
    "data/ship2.png"
    "data/ship3.png"
    "data/ship4.png"
    "data/waves.png"
    "main.qml"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BattleShip
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BattleShip APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BattleShip SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BattleShip
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(BattleShip
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(BattleShip
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt6::QuickControls2)

set_target_properties(BattleShip PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(BattleShip)
    qt_finalize_executable(BattleShip)
endif()
