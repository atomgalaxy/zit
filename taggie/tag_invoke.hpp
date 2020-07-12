/**
 * Adapted from unifex.
 */
#pragma once
#include <type_traits>
#include <utility>

#if defined(__GNUG__)
#define always_inline [[gnu::always_inline]]
#elif defined(__MSC_VER)
#define always_inline __inline
#else
#define always_inline
#endif

namespace taggie {
namespace _tag_invoke {
void tag_invoke();

struct _fn {
  template <typename CPO, typename... Args>
  constexpr auto operator()(CPO cpo, Args &&... args) const
      noexcept(noexcept(tag_invoke((CPO &&) cpo, (Args &&) args...)))
          -> decltype(tag_invoke((CPO &&) cpo, (Args &&) args...)) {
    return tag_invoke((CPO &&) cpo, (Args &&) args...);
  }
};

template <typename CPO, typename... Args>
using tag_invoke_result_t =
    decltype(tag_invoke(std::declval<CPO &&>(), std::declval<Args &&>()...));

struct yes_type {
  char dummy;
};
struct no_type {
  char dummy[2];
};

template <typename CPO, typename... Args>
auto try_tag_invoke(int) //
    noexcept(noexcept(tag_invoke(std::declval<CPO &&>(),
                                 std::declval<Args &&>()...)))
        -> decltype(static_cast<void>(tag_invoke(std::declval<CPO &&>(),
                                                 std::declval<Args &&>()...)),
                    yes_type{});

template <typename CPO, typename... Args>
no_type try_tag_invoke(...) noexcept(false);

template <template <typename...> class T, typename... Args>
struct defer {
  using type = T<Args...>;
};

struct empty {};
} // namespace _tag_invoke

inline namespace _tag_invoke_cpo {
inline constexpr _tag_invoke::_fn tag_invoke{};
}

template <auto &CPO>
using tag_t = std::remove_cvref_t<decltype(CPO)>;

// Manually implement the traits here rather than defining them in terms of
// the corresponding std::invoke_result/is_invocable/is_nothrow_invocable
// traits to improve compile-times. We don't need all of the generality of
// the std:: traits and the tag_invoke traits are used heavily through
// libunifex so optimising them for compile time makes a big difference.

using _tag_invoke::tag_invoke_result_t;

template <typename CPO, typename... Args>
inline constexpr bool is_tag_invocable_v =
    (sizeof(_tag_invoke::try_tag_invoke<CPO, Args...>(0)) ==
     sizeof(_tag_invoke::yes_type));

template <typename CPO, typename... Args>
struct tag_invoke_result
    : std::conditional_t<is_tag_invocable_v<CPO, Args...>,
                         _tag_invoke::defer<tag_invoke_result_t, CPO, Args...>,
                         _tag_invoke::empty> {};

template <typename CPO, typename... Args>
using is_tag_invocable = std::bool_constant<is_tag_invocable_v<CPO, Args...>>;

template <typename CPO, typename... Args>
inline constexpr bool is_nothrow_tag_invocable_v =
    noexcept(_tag_invoke::try_tag_invoke<CPO, Args...>(0));

template <typename CPO, typename... Args>
using is_nothrow_tag_invocable =
    std::bool_constant<is_nothrow_tag_invocable_v<CPO, Args...>>;

template <typename T, typename U>
using copy_rvref_t = std::conditional_t<std::is_rvalue_reference_v<T>,
                                        std::add_rvalue_reference_t<U>, U>;
template <typename T, typename U>
using copy_lvref_t = std::conditional_t<std::is_lvalue_reference_v<T>,
                                        std::add_lvalue_reference_t<U>, U>;
template <typename T, typename U>
using copy_ref_t = copy_rvref_t<T, copy_lvref_t<T, U>>;
template <typename T, typename U>
using copy_const_t =
    std::conditional_t<std::is_const_v<std::remove_reference_t<T>>,
                       std::add_const_t<U>, U>;
template <typename T, typename U>
using copy_volatile_t =
    std::conditional_t<std::is_volatile_v<std::remove_reference_t<T>>,
                       std::add_volatile_t<U>, U>;
template <typename T, typename U>
using copy_cvref_t = copy_ref_t<T, copy_const_t<T, copy_volatile_t<T, U>>>;

template <typename T, typename U>
always_inline inline constexpr auto forward_like(U &&x) noexcept
    -> copy_cvref_t<T, std::remove_reference_t<U>> {
  return std::forward<copy_cvref_t<T, std::remove_reference_t<U>>>(x);
}

template <typename T, typename U>
always_inline inline constexpr auto forward_cast(U &&x) noexcept
    -> copy_cvref_t<U &&, std::remove_cvref_t<T>> {
  return (copy_cvref_t<U &&, std::remove_cvref_t<T>>)x;
}

} // namespace taggie

#undef always_inline
