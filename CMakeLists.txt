cmake_minimum_required(VERSION 3.1.0)
project(zit VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(taggie INTERFACE)
target_include_directories(taggie INTERFACE ./)
target_sources(taggie INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/taggie/tag_invoke.hpp)
set_target_properties(taggie PROPERTIES INTERFACE_COMPILE_FEATURES cxx_std_20)

add_library(zit INTERFACE)
target_include_directories(zit INTERFACE ./)
target_sources(zit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/zit/zit.hpp)
set_target_properties(zit PROPERTIES INTERFACE_COMPILE_FEATURES cxx_std_20)
target_link_libraries(zit INTERFACE taggie)

add_subdirectory(test)
add_subdirectory(cars)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

