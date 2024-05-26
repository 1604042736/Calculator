project(Tests)

add_executable(TestInteger ${PROJECT_SOURCE_DIR}/TestInteger.cpp)
target_link_libraries(TestInteger Core)
add_test(NAME TestInteger COMMAND TestInteger)
set_tests_properties(TestInteger PROPERTIES TIMEOUT 1000)

add_executable(TestFloat ${PROJECT_SOURCE_DIR}/TestFloat.cpp)
target_link_libraries(TestFloat Core)
add_test(NAME TestFloat COMMAND TestFloat)
set_tests_properties(TestFloat PROPERTIES TIMEOUT 1000)

add_executable(TestRational ${PROJECT_SOURCE_DIR}/TestRational.cpp)
target_link_libraries(TestRational Core)
add_test(NAME TestRational COMMAND TestRational)
set_tests_properties(TestRational PROPERTIES TIMEOUT 1000)