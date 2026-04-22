find_package(Qt6 REQUIRED COMPONENTS Test)

qt_add_executable(tst_modellayer
    testModelLayer.cpp
)

target_link_libraries(tst_modellayer
    PRIVATE
    Qt6::Test
)

target_include_directories(tst_modellayer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Link the model source files directly into the test
target_sources(tst_modellayer PRIVATE
    ${CMAKE_SOURCE_DIR}/src/model/scriptelement.cpp
    ${CMAKE_SOURCE_DIR}/src/model/scene.cpp
    ${CMAKE_SOURCE_DIR}/src/model/script.cpp
)

include(CTest)
add_test(NAME tst_modellayer COMMAND tst_modellayer)
