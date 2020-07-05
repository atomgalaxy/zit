cc_library(
    name = "cars",
    deps = ["@zit"],
    hdrs=["cars.hpp"],
)

cc_library(
    name = "cars_impl_1",
    srcs = ["cars_impl_1.cpp"],
    deps = ["cars"],
    linkstatic=True,
)

cc_library(
    name = "cars_impl_2",
    srcs = ["cars_impl_2.cpp"],
    deps = ["cars"],
    linkstatic=True,
)

cc_library(
    name = "cars_main",
    srcs = ["cars_main.cpp"],
    deps = ["cars"],
    linkstatic=True,
)

cc_binary(
    name = "cars_1",
    srcs=[
        "cars_impl_1.cpp",
    ],
    deps=[
        "cars_main",
    ]
)

cc_binary(
    name = "cars_2",
    srcs=[
        "cars_impl_2.cpp",
    ],
    deps = [
        "cars_main",
    ],
)