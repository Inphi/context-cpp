set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/context)

file(GLOB LIB_SOURCES ${INCLUDE_DIR}/*.hpp)

set(TEST ${CMAKE_CURRENT_LIST_DIR}/basic.cc)

add_executable(test-basic ${LIB_SOURCES} ${TEST})

set_target_properties(test-basic PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)

target_link_libraries(test-basic PRIVATE contextcpp)

add_test(NAME context-tests COMMAND test-basic)