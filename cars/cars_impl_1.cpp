#include "cars.hpp"

namespace cars::_impl_lada {

struct lada {
  // zit::handle requirements
  int times;
};
auto tag_invoke(zit::construct_t, lada *l, int times) -> lada * {
  return zit::construct.default_impl(l, times);
}
auto tag_invoke(zit::construct_t, lada *l, lada const &other) -> lada * {
  return zit::construct.default_impl(l, other);
}
auto tag_invoke(zit::destroy_t, lada *x) -> void {
  zit::destroy.default_impl(x);
}

// other ops
auto tag_invoke(cars::honk_t, lada const &self, std::ostream &o) -> std::ostream & {
  for (int i = 0; i < self.times; ++i) {
    if (i != 0) {
      o << " ";
    }
    o << "meeeeep";
  }
  return o;
}

} // namespace cars::_impl_lada