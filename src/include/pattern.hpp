#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <string>
#include <string_view>

#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class Pattern {
 private:
  detail::Yarn<> m_pattern;

 public:
  template <std::size_t N>
  explicit consteval Pattern(const char (&str)[N]) : m_pattern(str) {}

  explicit consteval Pattern(char c) : m_pattern(c) {}

  explicit consteval Pattern(const std::string& s) : m_pattern(std::move(s)) {}

  [[nodiscard]] consteval std::string_view str() const {
    return this->m_pattern.view();
  }

  [[nodiscard]] consteval Pattern OneOrMore(std::string_view str) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
    return *this;
  }

  [[nodiscard]] consteval Pattern One(std::string_view str) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}]"), str));
    return *this;
  }

  [[nodiscard]] consteval Pattern Group(const Pattern& pattern) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("({})"), pattern.str()));
    return *this;
  }
};
}  // namespace stockholm

[[nodiscard]] consteval std::string Capture(const stockholm::Pattern& pattern) {
  return std::string(pattern.str());
}

[[nodiscard]] consteval stockholm::Pattern Group(
    const stockholm::Pattern& pattern) {
  return stockholm::Pattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("({})"), pattern.str()));
}

template <std::size_t N>
[[nodiscard]] consteval stockholm::Pattern OneOrMore(const char (&str)[N]) {
  return stockholm::Pattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
}
#endif  // !STOCKHOLM_PATTERN_HPP
