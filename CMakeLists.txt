cmake_minimum_required(VERSION 3.16)

project(image_compression_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(appimage_compression_app
    main.cpp
    qml.qrc
)

qt_add_qml_module(appimage_compression_app
    URI image_compression_app
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES
        RESOURCES
        RESOURCES qml.qrc
        RESOURCES
        QML_FILES
        QML_FILES resources/right_side/RightSide.qml
        QML_FILES resources/left_side/LeftSide.qml
        RESOURCES
        RESOURCES
        SOURCES getfilepath.h getfilepath.cpp
        SOURCES buttonsignalshandling.h buttonsignalshandling.cpp
        SOURCES processedimage.h processedimage.cpp
        SOURCES convertimage.h convertimage.cpp
)

target_link_libraries(appimage_compression_app
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

include(GNUInstallDirs)
install(TARGETS appimage_compression_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
