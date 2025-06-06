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
  consteval Pattern() = default;

  explicit consteval Pattern(const char c) : m_pattern(c) {}

  explicit consteval Pattern(const std::string& s) : m_pattern(std::move(s)) {}

  [[nodiscard]] consteval std::string_view str() const {
    return this->m_pattern.view();
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern ZeroOrMore(T&& str) {
    m_pattern.append(
        detail::constexpr_fmt(FMT_COMPILE("{}*"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern One(T&& str) {
    m_pattern.append(
        detail::constexpr_fmt(FMT_COMPILE("{}"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern OneOrMore(T&& str) {
    m_pattern.append(
        detail::constexpr_fmt(FMT_COMPILE("{}+"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern Optional(T&& str) {
    m_pattern.append(
        detail::constexpr_fmt(FMT_COMPILE("{}?"), std::string_view(str)));
    return *this;
  }

  [[nodiscard]] consteval Pattern Group(const Pattern& pattern) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("({})"), pattern.str()));
    return *this;
  }

  // Possibly include this?
  //   auto grouped = Pattern{};
  //   grouped.m_pattern.append(
  //       detail::constexpr_fmt(FMT_COMPILE("({})"), this->str()));
  //   return grouped;
  // }
};
}  // namespace stockholm

[[nodiscard]] consteval std::string Capture(const stockholm::Pattern& pattern) {
  return std::string(pattern.str());
}

[[nodiscard]] consteval stockholm::Pattern Start() {
  return stockholm::Pattern{};
}

#endif  // !STOCKHOLM_PATTERN_HPP
