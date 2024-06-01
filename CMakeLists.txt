cmake_minimum_required(VERSION 3.23.0-rc3)

project(Calculator VERSION 1.0.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

string(TIMESTAMP COMPILE_TIME "%Y-%m-%d,%H:%M:%S")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ADDRESS_MODEL 64)
else()
    set(ADDRESS_MODEL 32)
endif()
configure_file(Config.h.in ${PROJECT_SOURCE_DIR}/Include/Config.h)

file(GLOB_RECURSE OBJECTS_SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Objects/*.cpp)
file(GLOB_RECURSE COMPILER_SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Compiler/*.cpp)
file(GLOB_RECURSE COMMON_SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Common/*.cpp)

set(CORE_SOURCE ${OBJECTS_SOURCE} ${COMPILER_SOURCE} ${COMMON_SOURCE})

add_library(Core SHARED ${CORE_SOURCE})
target_include_directories(Core PUBLIC ${PROJECT_SOURCE_DIR}/Include)

add_executable(Calculator Main.cpp)
target_link_libraries(Calculator Core)

enable_testing()

add_subdirectory(Tests)
add_subdirectory(Apps)