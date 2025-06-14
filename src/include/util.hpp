#ifndef STOCKHOLM_UTIL_HPP
#define STOCKHOLM_UTIL_HPP

#include <fmt/compile.h>

#include <string>

namespace stockholm::detail {
template <typename T>
concept StrLike = std::convertible_to<T, std::string_view>;

template <typename Format, typename... Args>
[[nodiscard]] consteval std::string constexpr_fmt(Format format,
                                                  Args &&...args) {
  char c[64] = {};
  fmt::format_to(c, format, std::forward<Args>(args)...);
  return {c};
}
}  // namespace stockholm::detail

#endif  // !STOCKHOLM_UTIL_HPP
