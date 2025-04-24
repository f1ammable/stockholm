#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <concepts>
#include <string>
#include <string_view>

#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class BasePattern {
 protected:
  detail::Yarn<> m_pattern;

  template <std::size_t N>
  explicit consteval BasePattern(const char (&str)[N]) : m_pattern(str) {}

  explicit consteval BasePattern(char c) : m_pattern(c) {}

  explicit consteval BasePattern(const std::string& s)
      : m_pattern(std::move(s)) {}

 public:
  virtual ~BasePattern() = default;
  BasePattern(const BasePattern&) = delete;
  BasePattern& operator=(const BasePattern&) = delete;
  BasePattern(BasePattern&&) = delete;
  BasePattern& operator=(BasePattern&&) = delete;

  [[nodiscard]] consteval std::string_view pattern() const {
    return this->m_pattern.view();
  }
};

class OneOrMorePattern final : public BasePattern {
 public:
  template <std::size_t N>
  explicit consteval OneOrMorePattern(const char (&str)[N])
      : BasePattern(str) {}

  explicit consteval OneOrMorePattern(const std::string& s) : BasePattern(s) {}
};

}  // namespace stockholm

template <typename T>
  requires std::derived_from<T, stockholm::BasePattern>
[[nodiscard]] consteval std::string Capture(const T& pattern) {
  return std::string(pattern.pattern());
}

template <std::size_t N>
[[nodiscard]] consteval stockholm::OneOrMorePattern OneOrMore(
    const char (&str)[N]) {
  return stockholm::OneOrMorePattern(
      stockholm::detail::constexpr_fmt(FMT_COMPILE("[{}]+"), str));
}
#endif  // !STOCKHOLM_PATTERN_HPP
