add_executable(cars_impl_1 cars_impl_1.cpp cars_main.cpp)
target_link_libraries(cars_impl_1 zit)

add_executable(cars_impl_2 cars_impl_2.cpp cars_main.cpp)
target_link_libraries(cars_impl_2 zit)

add_test(NAME cars_impl_1 COMMAND cars_impl_1)
add_test(NAME cars_impl_2 COMMAND cars_impl_2)
