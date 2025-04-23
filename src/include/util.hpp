#ifndef STOCKHOLM_UTIL_HPP
#define STOCKHOLM_UTIL_HPP

#include <fmt/compile.h>

#include <string>

namespace stockholm::detail {
template <typename Format, typename... Args>
[[nodiscard]] inline consteval std::string constexpr_fmt(Format format,
                                                         Args &&...args) {
  char c[64] = {};
  fmt::format_to(c, format, std::forward<Args>(args)...);
  return std::string(c);
}
}  // namespace stockholm::detail

#endif  // !STOCKHOLM_UTIL_HPP
