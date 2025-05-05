#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <string>
#include <string_view>

#include "selector.hpp"
#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class Pattern {
 private:
  detail::Yarn<> m_pattern;

 public:
  template <std::size_t N>

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

template <typename T>
concept StrLiteral =
    std::is_array_v<std::remove_reference_t<T>> &&
    std::same_as<std::remove_all_extents_t<std::remove_reference_t<T>>,
                 const char>;

template <typename T>
  requires StrLiteral<T>
[[nodiscard]] consteval stockholm::Pattern OneOrMore(T&& s) {
  return stockholm::Pattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), s));
}

template <Selector S>
[[nodiscard]] consteval stockholm::Pattern OneOrMore(
    const Matcher<S>& pattern) {
  return stockholm::Pattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), pattern.str()));
}

#endif  // !STOCKHOLM_PATTERN_HPP
