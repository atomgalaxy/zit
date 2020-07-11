#pragma once

#include "taggie/tag_invoke.hpp"

#include <memory>
#include <type_traits>

namespace zit {

#define ZIT_PRIVATE_TAG_METH(type, name, base, constness)                      \
  template <typename... As>                                                    \
  auto name(As &&... as)                                                       \
      constness noexcept(noexcept(std::declval<type>()(                        \
                             std::declval<base constness>(), (As &&)(as)...))) \
          ->decltype(std::declval<type>()(std::declval<base constness>(),      \
                                          (As &&)(as)...)) {                   \
    return type{}(static_cast<base constness>(*this), (As &&)(as)...);         \
  }                                                                            \
  static_assert(true, "require semi at end of method")

/**
 * Define a customization point object (CPO) which can be used with `handle`.
 *
 * Example:
 *
 * ```cpp
 * namespace my_namespace {
 *   ZIT_CPO(honk_t, honk); // honk_t is the type, honk the name.
 *   ZIT_CPO(set_throttle_t, set_throttle);
 * }
 * namespace possibly_other_namespace {
 *   struct my_impl; // forward declare
 *   struct my_handle : zit::handle<my_impl,
 *                                  my_namespace::honk,
 *                                  my_namespace::set_throttle> {};
 * }
 * ```
 */
#define ZIT_CPO(type, obj)                                                     \
  inline constexpr struct type final {                                         \
    template <typename... As>                                                  \
    inline constexpr auto operator()(As &&... as) const                        \
        noexcept(::taggie::is_nothrow_tag_invocable_v<type, As &&...>)            \
            -> ::taggie::tag_invoke_result_t<type, As &&...> {                    \
      return ::taggie::tag_invoke(*this, (As &&) as...);                          \
    }                                                                          \
    template <typename Base>                                                   \
    struct crtp {                                                              \
      ZIT_PRIVATE_TAG_METH(type, obj, Base, &);                                \
      ZIT_PRIVATE_TAG_METH(type, obj, Base, const &);                          \
      ZIT_PRIVATE_TAG_METH(type, obj, Base, &&);                               \
      ZIT_PRIVATE_TAG_METH(type, obj, Base, const &&);                         \
    };                                                                         \
  } obj{};                                                                     \
  static_assert("require semicolon at end of macro")

inline constexpr struct construct_t final {
  template <typename... As>
  inline constexpr auto operator()(As &&... as) const
      noexcept(::taggie::is_nothrow_tag_invocable_v<construct_t, As &&...>)
          -> ::taggie::tag_invoke_result_t<construct_t, As &&...> {
    return ::taggie::tag_invoke(*this, (As &&) as...);
  }

  template <typename Impl, typename... As>
  static constexpr auto
  default_impl(Impl * /*always nullptr*/,
               As &&... as) noexcept(noexcept(new Impl((As &&) as...)))
      -> decltype(new Impl((As &&) as...)) {
    return new Impl((As &&) as...);
  }
} construct{};

inline constexpr struct destroy_t final {
  template <typename... As>
  inline constexpr auto operator()(As &&... as) const
      noexcept(::taggie::is_nothrow_tag_invocable_v<destroy_t, As &&...>)
          -> ::taggie::tag_invoke_result_t<destroy_t, As &&...> {
    return ::taggie::tag_invoke(*this, (As &&) as...);
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
  template <typename... Args>
  handle(Args &&... args)
      : _impl(::zit::construct((Impl *)nullptr, (Args &&) args...)) {}

  handle(handle const &other) : handle(*other._impl) {}

  auto operator=(handle const &other) -> handle & {
    handle copy = other;
    std::swap(_impl, copy._impl);
    return *this;
  }

  template <typename CPO, typename H, typename... As>
  friend inline auto tag_invoke(CPO cpo, H &&h, As &&... as) noexcept(
      noexcept(cpo(::taggie::forward_like<H &&>(*h._impl), (As &&) as...)))
      -> decltype(cpo(taggie::forward_like<H &&>(*h._impl), (As &&) as...)) {
    return cpo(taggie::forward_like<H &&>(*h._impl), (As &&) as...);
  }

private:
  std::unique_ptr<Impl, zit::destroy_t> _impl;
};

} // namespace zit