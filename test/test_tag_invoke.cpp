/// Just tests that the tag_invoke extracted from unifex works.

#include "taggie/tag_invoke.hpp"

#include <iostream>
#include <type_traits>

inline constexpr struct cpo_t final {
  template <typename... As>
  inline constexpr auto operator()(As &&... as) const
      noexcept(taggie::is_nothrow_tag_invocable_v<cpo_t, As&&...>)
          -> ::taggie::tag_invoke_result_t<cpo_t, As&&...> {
    return ::taggie::tag_invoke(*this, (As &&)as...);
  }
} cpo;

struct foo {
  friend constexpr auto tag_invoke(cpo_t, foo const &) noexcept { return 1; }
};

static_assert(std::is_same_v<int, decltype(cpo(foo{}))>);

int main() {}