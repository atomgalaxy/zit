add_executable(test_tag_invoke test_tag_invoke.cpp)
target_link_libraries(test_tag_invoke taggie)

add_test(NAME test_tag_invoke COMMAND test_tag_invoke)
