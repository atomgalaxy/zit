#pragma once

#include "tag_invoke.hpp"

#include <memory>
#include <type_traits>

namespace zit {

#define ZIT_TAG_METH(type, name, base, constness)                              \
  template <typename... As>                                                    \
  auto name(As &&... as) constness noexcept(                                   \
      noexcept(std::declval<type>()(std::declval<base constness>(),            \
                                    std::forward<As>(as)...)))                 \
      ->decltype(std::declval<type>()(std::declval<base constness>(),          \
                                      std::forward<As>(as)...)) {              \
    return type{}(static_cast<base constness>(*this),                          \
                  std::forward<As>(as)...);                                    \
  }                                                                            \
  static_assert(true, "require semi at end of method")

#define ZIT_TAG_INVOKE_CPO(type, obj)                                          \
  inline constexpr struct type final {                                         \
    template <typename... As>                                                  \
    inline constexpr auto operator()(As &&... as) const                        \
        noexcept(zit::is_nothrow_tag_invocable_v<type, As &&...>)              \
            -> zit::tag_invoke_result_t<type, As &&...> {                      \
      return zit::tag_invoke(*this, (As &&) as...);                            \
    }                                                                          \
    template <typename Base>                                                   \
    struct crtp {                                                              \
      ZIT_TAG_METH(type, obj, Base, &);                                        \
      ZIT_TAG_METH(type, obj, Base, const &);                                  \
      ZIT_TAG_METH(type, obj, Base, &&);                                       \
      ZIT_TAG_METH(type, obj, Base, const &&);                                 \
    };                                                                         \
  } obj{};                                                                     \
  static_assert("require semicolon at end of macro")

inline constexpr struct construct_t final {
  template <typename... As>
  inline constexpr auto operator()(As &&... as) const
      noexcept(zit::is_nothrow_tag_invocable_v<construct_t, As &&...>)
          -> zit::tag_invoke_result_t<construct_t, As &&...> {
    return zit::tag_invoke(*this, (As &&) as...);
  }
  template <typename Impl, typename... As>
  static constexpr auto default_impl(
      Impl * /*always nullptr*/,
      As &&... as) noexcept(noexcept(new Impl(std::forward<As>(as)...)))
      -> decltype(new Impl(std::forward<As>(as)...)) {
    return new Impl(std::forward<As>(as)...);
  }
} construct{};
inline constexpr struct destroy_t final {
  template <typename... As>
  inline constexpr auto operator()(As &&... as) const
      noexcept(zit::is_nothrow_tag_invocable_v<destroy_t, As &&...>)
          -> zit::tag_invoke_result_t<destroy_t, As &&...> {
    return zit::tag_invoke(*this, (As &&) as...);
  }
  template <typename Impl>
  static constexpr auto default_impl(Impl *obj) {
    delete obj;
  }
} destroy{};

template <typename Base, auto const &cpo>
using crtp_base =
    typename std::remove_cvref_t<decltype(cpo)>::template crtp<Base>;

template <typename Impl, auto const &... cpos>
struct handle : crtp_base<handle<Impl, cpos...>, cpos>... {
  handle(auto &&... args)
      : _impl(zit::construct((Impl *)nullptr,
                             std::forward<decltype(args)>(args)...)) {}

  handle(handle const &other) : handle(*other._impl) {}

  auto operator=(handle const &other) -> handle & {
    handle copy = other;
    std::swap(_impl, copy._impl);
    return *this;
  }

  friend inline auto tag_invoke(auto cpo, auto &&h, auto &&... as) noexcept(
      noexcept(cpo(zit::forward_like<decltype(h)>(*h._impl),
                   std::forward<decltype(as)>(as)...)))
      -> decltype(cpo(zit::forward_like<decltype(h)>(*h._impl),
                      std::forward<decltype(as)>(as)...)) {
    return cpo(zit::forward_like<decltype(h)>(*h._impl),
               std::forward<decltype(as)>(as)...);
  }

private:
  std::unique_ptr<Impl, zit::destroy_t> _impl;
};

} // namespace zit