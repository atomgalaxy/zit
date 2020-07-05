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

- [tag invoke](zit/tag_invoke.hpp) shamelessly taken from libunifex and abbreviated.
- [zit](zit/zit.hpp) is the library to enable the "efficient" generation of pimpl wrappers.
- [cars.hpp](cars/cars.hpp) is the common header where the consumer and producer of the pimpl abstraction agree on a public interface. Notice that `struct lada;` is only ever forward-declared, with its public interface completely represented by free functions.
- [cars_impl_1](cars/cars_impl_1.cpp) and [cars_impl_2](cars/cars_impl_2.cpp) are the two pimpl implementations.
- [cars main](cars/cars_main.cpp) exercises the `h.honk(std::cout)` method.
