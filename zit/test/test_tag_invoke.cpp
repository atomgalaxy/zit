/// Just tests that the tag_invoke extracted from unifex works.

#include "../tag_invoke.hpp"

#include <iostream>
#include <type_traits>

inline constexpr struct cpo_t final {
  inline constexpr auto operator()(auto &&... as) const
      noexcept(zit::is_nothrow_tag_invocable_v<cpo_t, decltype(as)...>)
          -> zit::tag_invoke_result_t<cpo_t, decltype(as)...> {
    return zit::tag_invoke(*this, (decltype(as) &&)as...);
  }
} cpo;

struct foo {
  friend constexpr auto tag_invoke(cpo_t, foo const &) noexcept { return 1; }
};

static_assert(std::is_same_v<int, decltype(cpo(foo{}))>);

int main() {}