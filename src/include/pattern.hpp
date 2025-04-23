#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>

#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class BasePattern {
 protected:
  detail::Yarn<> m_pattern;

  template <std::size_t N>
  inline explicit consteval BasePattern(const char (&str)[N])
      : m_pattern(str) {}

  inline explicit consteval BasePattern(char c) : m_pattern(c) {}

  inline explicit consteval BasePattern(std::string s)
      : m_pattern(std::move(s)) {}

 public:
  virtual ~BasePattern() = default;
  BasePattern(const BasePattern&) = delete;
  BasePattern& operator=(const BasePattern&) = delete;
  BasePattern(BasePattern&&) = delete;
  BasePattern& operator=(BasePattern&&) = delete;

  [[nodiscard]] inline consteval std::string_view pattern() const {
    return this->m_pattern.view();
  }
};

class OneOrMorePattern : public BasePattern {
 public:
  template <std::size_t N>
  inline explicit consteval OneOrMorePattern(const char (&str)[N])
      : BasePattern(str) {}

  inline explicit consteval OneOrMorePattern(std::string s)
      : BasePattern(std::move(s)) {}
};

}  // namespace stockholm

template <typename T>
  requires std::derived_from<T, stockholm::BasePattern>
[[nodiscard]] inline consteval std::string Capture(const T& pattern) {
  return std::string(pattern.pattern());
}

template <std::size_t N>
[[nodiscard]] inline consteval stockholm::OneOrMorePattern OneOrMore(
    const char (&str)[N]) {
  return stockholm::OneOrMorePattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
}
#endif  // !STOCKHOLM_PATTERN_HPP
