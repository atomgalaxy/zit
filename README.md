zit
===

Zit is an experimental pimpl-on-top-of-tag-invoke prototype.

Building and demo
-----------------

```
┌─[atomicity@kepler] - [~/work/code/tag_invoke_pimpl] - [2020-07-05 11:44:53]
└─[0] <git:(master✱+✈) > bazel build -c opt //...
INFO: Analyzed 8 targets (1 packages loaded, 10 targets configured).
INFO: Found 8 targets...
INFO: Elapsed time: 0.744s, Critical Path: 0.69s
INFO: 8 processes: 8 linux-sandbox.
INFO: Build completed successfully, 11 total actions
┌─[atomicity@kepler] - [~/work/code/tag_invoke_pimpl] - [2020-07-05 11:46:20]
└─[0] <git:(master✱+✈) > ./bazel-bin/cars/cars_1
meeeeep meeeeep meeeeep meeeeep meeeeep
┌─[atomicity@kepler] - [~/work/code/tag_invoke_pimpl] - [2020-07-05 11:46:29]
└─[0] <git:(master✱+✈) > ./bazel-bin/cars/cars_2
moooop moooop moooop moooop moooop
```

Implementation
--------------

**Library:**

- [tag invoke](taggie/tag_invoke.hpp) shamelessly taken from libunifex and abbreviated.
- [zit](zit/zit.hpp) is the library to enable the "efficient" generation of pimpl wrappers.
- [cars.hpp](cars/cars.hpp) is the common header where the consumer and producer of the pimpl abstraction agree on a public interface. Notice that `struct lada;` is only ever forward-declared, with its public interface completely represented by free functions.
- [cars_impl_1](cars/cars_impl_1.cpp) and [cars_impl_2](cars/cars_impl_2.cpp) are the two pimpl implementations.
- [cars main](cars/cars_main.cpp) exercises the `h.honk(std::cout)` method.

Quick Intro
-----------

1. include `zit/zit.hpp`.
2. make a header with a bunch of customization points

    ```cpp
    namespace my_namespace {
        ZIT_CPO(honk_t, honk); // honk_t is the type, honk the name.
        ZIT_CPO(set_throttle_t, set_throttle);
    }
    ```

3. Define your type and its interface

    ```cpp
    namespace my_object_namespace {
        struct my_impl; // forward declare the type
        // constructors and destructor are mandatory
        auto tag_invoke(zit::construct, my_impl*, int) -> my_impl*;
        auto tag_invoke(zit::construct, my_impl*, my_impl const&) -> my_impl*; // copy
        void tag_invoke(zit::destroy, my_impl*); // destructor interface

        // forward declate customization point implementations
        void tag_invoke(my_namespace::honk_t, int n, std::ostream& out);
        void tag_invoke(my_namespace::honk_t, std::ostream& out); // default n
        void tag_invoke(my_namespace::set_throttle_t, int percent);
    }
    ```

4. Make handle for your impl

    ```cpp
    namespace possibly_other_namespace {
        struct my_handle : zit::handle<my_namespace::my_impl,
                                       my_namespace::honk,
                                       my_namespace::set_throttle> {};
    }
    ```

5. Use it!

    ```cpp
    int main() {
        auto h = possibly_other_namespace::my_handle(5);
        // works with free function syntax
        my_namespace::honk(h, 5, std::cout);
        // as well as with method syntax
        h.honk(std::cout);
        // copying also works out of the box
        auto h2 = h;
    }
    ```