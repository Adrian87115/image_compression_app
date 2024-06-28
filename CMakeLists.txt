cmake_minimum_required(VERSION 3.16)

project(image_compression_app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

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
)

target_link_libraries(appimage_compression_app
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appimage_compression_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
