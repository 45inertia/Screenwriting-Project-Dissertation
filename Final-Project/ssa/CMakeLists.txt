cmake_minimum_required(VERSION 3.16)

project(ssa VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml PrintSupport)

set(PROJECT_SOURCES
        src/main.cpp

        # view
        src/view/mainwindow.cpp
        src/view/mainwindow.h
        src/view/mainwindow.ui
        src/view/scenenavigator.h
        src/view/scenenavigator.cpp
        src/view/scripteditor.h
        src/view/scripteditor.cpp
        src/view/newscriptdialog.cpp
        src/view/newscriptdialog.h
        src/view/welcomescreen.h
        src/view/welcomescreen.cpp

        # models
        src/model/script.cpp
        src/model/script.h
        src/model/scene.cpp
        src/model/scene.h
        src/model/scriptelement.cpp
        src/model/scriptelement.h
        src/model/ElementType.h

        # viewmodel
        src/viewmodel/scriptviewmodel.h
        src/viewmodel/scriptviewmodel.cpp

        # services
        src/services/scriptmanager.h
        src/services/scriptmanager.cpp
        src/services/osfserializer.h
        src/services/osfserializer.cpp
        src/services/pdfexporter.h
        src/services/pdfexporter.cpp
        src/services/characterregister.h
        src/services/characterregister.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ssa
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/view/newscriptdialog.h src/view/newscriptdialog.cpp
        src/view/welcomescreen.h src/view/welcomescreen.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ssa APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ssa SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ssa
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(ssa PRIVATE src)

target_link_libraries(ssa PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ssa)
endif()
set_target_properties(ssa PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ssa
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ssa)
endif()

add_subdirectory(tests)
