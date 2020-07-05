#include "../zit/tag_invoke.hpp"
#include "../zit/zit.hpp"

#include <memory>
#include <ostream>

namespace cars {

ZIT_TAG_INVOKE_CPO(honk_t, honk);

namespace _impl_lada {
struct lada;
// zit::handle requirements
auto tag_invoke(zit::construct_t, lada *, int) -> lada *;
auto tag_invoke(zit::construct_t, lada *, lada const &) -> lada *;
auto tag_invoke(zit::destroy_t, lada *) -> void;

// other ops
auto tag_invoke(cars::honk_t, lada const &, std::ostream &) -> std::ostream &;
} // namespace _impl_lada

struct lada : zit::handle<_impl_lada::lada, cars::honk> {};

} // namespace cars