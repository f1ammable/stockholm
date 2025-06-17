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

  consteval Pattern() = default;

  explicit consteval Pattern(const char c) : m_pattern(c) {}

  explicit consteval Pattern(const std::string& s) : m_pattern(std::move(s)) {}

  explicit consteval Pattern(const detail::Yarn<>& str) : m_pattern(str) {}

 public:
  friend consteval Pattern Start();
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

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern Quantify(T&& str, int min, int max) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("{}{{{},{}}}"),
                                           std::string_view(str), min, max));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern Exactly(T&& str, int count) {
    m_pattern.append(
        detail::constexpr_fmt(FMT_COMPILE("{}{{{}}}"), str, count));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern AtLeast(T&& str, int min) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("{}{{{},}}"), str, min));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] consteval Pattern AtMost(T&& str, int max) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("{}{{,{}}}"), str, max));
    return *this;
  }

  [[nodiscard]] consteval Pattern CharacterClass(std::string_view chars) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}]"), chars));
    return *this;
  }

  [[nodiscard]] consteval Pattern Range(char from, char to) {
    m_pattern.append(detail::constexpr_fmt(FMT_COMPILE("[{}-{}]"), from, to));
    return *this;
  }

  [[nodiscard]] consteval Pattern operator+(const Pattern& rhs) const {
    return Pattern(this->m_pattern + rhs.m_pattern);
  }

  [[nodiscard]] consteval Pattern operator|(const Pattern& rhs) const {
    return Pattern(detail::constexpr_fmt(FMT_COMPILE("{}|{}"),
                                         this->m_pattern.view(), rhs.str()));
  }

  template <typename... Patterns>
    requires stockholm::detail::AllStrLike<Patterns...>
  [[nodiscard]] consteval Pattern OneOf(Patterns&&... patterns) {
    static_assert(sizeof...(patterns) >= 2,
                  "OneOf requires at least two patterns");
    return (patterns | ...);
  }
};  // namespace stockholm

[[nodiscard]] consteval stockholm::Pattern Start() {
  return stockholm::Pattern{};
}
}  // namespace stockholm

[[nodiscard]] consteval std::string Capture(const stockholm::Pattern& pattern) {
  return std::string(pattern.str());
}

#endif  // !STOCKHOLM_PATTERN_HPP
