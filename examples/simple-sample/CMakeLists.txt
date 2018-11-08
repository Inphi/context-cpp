add_executable(simple-sample ${CMAKE_CURRENT_LIST_DIR}/sample.cc)

target_include_directories(simple-sample PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(simple-sample PRIVATE contextcpp)

set_target_properties(simple-sample PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)