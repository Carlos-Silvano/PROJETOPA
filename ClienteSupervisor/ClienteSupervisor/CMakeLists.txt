cmake_minimum_required(VERSION 3.16)

project(ClienteSupervisor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Network PrintSupport)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qcustomplot.cpp
    qcustomplot.h
)


qt_add_executable(ClienteSupervisor
    ${PROJECT_SOURCES}
)


target_link_libraries(ClienteSupervisor PRIVATE
    Qt::Widgets
    Qt::Network
    Qt::PrintSupport
)
target_include_directories(ClienteSupervisor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})



set_target_properties(ClienteSupervisor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ClienteSupervisor
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ClienteSupervisor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
