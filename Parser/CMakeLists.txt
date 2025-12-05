cmake_minimum_required(VERSION 3.19)
project(TINYParserGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg Widgets SvgWidgets)

qt_standard_project_setup()

qt_add_executable(TINYParserGUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    parser.h parser.cpp
    graphviz_renderer.h graphviz_renderer.cpp
)

target_link_libraries(TINYParserGUI
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Svg          # QSvgRenderer
        Qt6::SvgWidgets   # QGraphicsSvgItem
)

include(GNUInstallDirs)

install(TARGETS TINYParserGUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TINYParserGUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
