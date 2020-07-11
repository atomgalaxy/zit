# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "taggie",
    hdrs = [
        "taggie/tag_invoke.hpp",
    ],
    visibility = ["//visibility:public"],
)
cc_library(
    name = "zit",
    hdrs = [
        "zit/zit.hpp"
    ],
    deps=["taggie"],
    visibility = ["//visibility:public"],
)

