cmake_minimum_required(VERSION 3.16)
project(ssa_tests CXX)
enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Test Widgets Xml)

# ─── Enable automatic MOC generation ─────────────────────────────────────────
set(CMAKE_AUTOMOC ON)

# ─── Model Layer Tests ────────────────────────────────────────────────────────
qt_add_executable(tst_modellayer
    testModelLayer.cpp
    ${CMAKE_SOURCE_DIR}/src/model/scriptelement.cpp
    ${CMAKE_SOURCE_DIR}/src/model/scene.cpp
    ${CMAKE_SOURCE_DIR}/src/model/script.cpp
)

target_link_libraries(tst_modellayer
    PRIVATE
    Qt6::Test
)

target_include_directories(tst_modellayer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME tst_modellayer COMMAND tst_modellayer)

# ─── Services Layer Tests (OsfSerializer etc.) ────────────────────────────────
# Uncomment when ready:
#
# qt_add_executable(tst_serviceslayer
#     testServicesLayer.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/scriptelement.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/scene.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/script.cpp
#     ${CMAKE_SOURCE_DIR}/src/services/osfserializer.cpp
#     ${CMAKE_SOURCE_DIR}/src/services/scriptmanager.cpp
# )
#
# target_link_libraries(tst_serviceslayer
#     PRIVATE
#     Qt6::Test
#     Qt6::Xml
# )
#
# target_include_directories(tst_serviceslayer PRIVATE
#     ${CMAKE_SOURCE_DIR}/src
# )
#
# add_test(NAME tst_serviceslayer COMMAND tst_serviceslayer)

# ─── ViewModel Layer Tests ────────────────────────────────────────────────────
# Uncomment when ready:
#
# qt_add_executable(tst_viewmodellayer
#     testViewModelLayer.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/scriptelement.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/scene.cpp
#     ${CMAKE_SOURCE_DIR}/src/model/script.cpp
#     ${CMAKE_SOURCE_DIR}/src/services/scriptmanager.cpp
#     ${CMAKE_SOURCE_DIR}/src/viewmodel/scriptviewmodel.cpp
# )
#
# target_link_libraries(tst_viewmodellayer
#     PRIVATE
#     Qt6::Test
#     Qt6::Widgets
# )
#
# target_include_directories(tst_viewmodellayer PRIVATE
#     ${CMAKE_SOURCE_DIR}/src
# )
#
# add_test(NAME tst_viewmodellayer COMMAND tst_viewmodellayer)

