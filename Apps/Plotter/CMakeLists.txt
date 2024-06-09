project(PlotterApp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Svg REQUIRED)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/*.ui)

add_executable(Plotter ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES})
target_link_libraries(Plotter Qt5::Widgets Qt5::Svg)
target_link_libraries(Plotter Core)
add_custom_command(TARGET Plotter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:Numerical>
    $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:Numerical>
    $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:Numerical>
    $<TARGET_FILE:Qt5::Svg> $<TARGET_FILE_DIR:Numerical>
)