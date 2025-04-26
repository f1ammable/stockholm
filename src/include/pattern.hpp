#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <string>
#include <string_view>

#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class BasePattern {
 private:
  detail::Yarn<> m_pattern;

 public:
  template <std::size_t N>
  explicit consteval BasePattern(const char (&str)[N]) : m_pattern(str) {}

  explicit consteval BasePattern(char c) : m_pattern(c) {}

  explicit consteval BasePattern(const std::string& s)
      : m_pattern(std::move(s)) {}

  [[nodiscard]] consteval std::string_view str() const {
    return this->m_pattern.view();
  }

  [[nodiscard]] consteval BasePattern OneOrMore(std::string_view str) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
    return *this;
  }

  [[nodiscard]] consteval BasePattern One(std::string_view str) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}]"), str));
    return *this;
  }
};
}  // namespace stockholm

[[nodiscard]] consteval std::string Capture(
    const stockholm::BasePattern& pattern) {
  return std::string(pattern.str());
}

// [[nodiscard]] consteval stockholm::BasePattern Group(const
// stockholm::BasePattern& pattern) {}

template <std::size_t N>
[[nodiscard]] consteval stockholm::BasePattern OneOrMore(const char (&str)[N]) {
  return stockholm::BasePattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
}
#endif  // !STOCKHOLM_PATTERN_HPP
