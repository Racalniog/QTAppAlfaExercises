cmake_minimum_required(VERSION 3.16)
project(QTAlfaExercises VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network Xml Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml Sql)

qt_standard_project_setup()

qt_add_executable(QTAlfaExercises WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    bmi.cpp
    styleconfigure.h styleconfigure.cpp styleconfigure.ui
    comboboxfont.h comboboxfont.cpp comboboxfont.ui
    rssreader.h rssreader.cpp rssreader.ui
)

target_link_libraries(QTAlfaExercises PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Network
    Qt::Xml
    Qt::Sql
)

install(TARGETS QTAlfaExercises
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET QTAlfaExercises
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
