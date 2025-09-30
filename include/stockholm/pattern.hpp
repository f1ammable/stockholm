#ifndef STOCKHOLM_PATTERN_HPP
#define STOCKHOLM_PATTERN_HPP

#include <fmt/compile.h>

#include <string>
#include <string_view>

#include "stockholm/format.hpp"
#include "util.hpp"
#include "yarn.hpp"

namespace stockholm {

class Pattern {
 private:
  detail::Yarn<> m_pattern;

  constexpr Pattern() = default;

  explicit constexpr Pattern(const char c) : m_pattern(c) {}

  explicit constexpr Pattern(const std::string& s) : m_pattern(std::move(s)) {}

  explicit constexpr Pattern(const detail::Yarn<>& str) : m_pattern(str) {}

 public:
  friend constexpr Pattern Start();
  [[nodiscard]] constexpr std::string_view str() const {
    return this->m_pattern.view();
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern ZeroOrMore(T&& str) {
    m_pattern.append(
        detail::format(COMPILE_FORMAT("{}*"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern One(T&& str) {
    m_pattern.append(
        detail::format(COMPILE_FORMAT("{}"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern OneOrMore(T&& str) {
    m_pattern.append(
        detail::format(COMPILE_FORMAT("{}+"), std::string_view(str)));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern Optional(T&& str) {
    m_pattern.append(
        detail::format(COMPILE_FORMAT("{}?"), std::string_view(str)));
    return *this;
  }

  [[nodiscard]] constexpr Pattern Group(const Pattern& pattern) {
    m_pattern.append(detail::format(COMPILE_FORMAT("({})"), pattern.str()));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern Quantify(T&& str, int min, int max) {
    m_pattern.append(detail::format(COMPILE_FORMAT("{}{{},{}}"),
                                    std::string_view(str), min, max));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern Exactly(T&& str, int count) {
    m_pattern.append(detail::format(COMPILE_FORMAT("{}{{}}"), str, count));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern AtLeast(T&& str, int min) {
    m_pattern.append(detail::format(COMPILE_FORMAT("{}{{},}"), str, min));
    return *this;
  }

  template <typename T>
    requires stockholm::detail::StrLike<T>
  [[nodiscard]] constexpr Pattern AtMost(T&& str, int max) {
    m_pattern.append(detail::format(COMPILE_FORMAT("{}{,{}}"), str, max));
    return *this;
  }

  [[nodiscard]] constexpr Pattern CharacterClass(std::string_view chars) {
    m_pattern.append(detail::format(COMPILE_FORMAT("[{}]"), chars));
    return *this;
  }

  [[nodiscard]] constexpr Pattern Range(char from, char to) {
    m_pattern.append(detail::format(COMPILE_FORMAT("[{}-{}]"), from, to));
    return *this;
  }

  [[nodiscard]] constexpr Pattern operator+(const Pattern& rhs) const {
    return Pattern(this->m_pattern + rhs.m_pattern);
  }

  [[nodiscard]] constexpr Pattern operator|(const Pattern& rhs) const {
    return Pattern(detail::format(COMPILE_FORMAT("{}|{}"),
                                  this->m_pattern.view(), rhs.str()));
  }

  template <typename... Patterns>
    requires stockholm::detail::AllStrLike<Patterns...>
  [[nodiscard]] constexpr Pattern OneOf(Patterns&&... patterns) {
    static_assert(sizeof...(patterns) >= 2,
                  "OneOf requires at least two patterns");
    return (patterns | ...);
  }
};

[[nodiscard]] constexpr Pattern Start() { return Pattern{}; }
}  // namespace stockholm

[[nodiscard]] constexpr std::string Capture(const stockholm::Pattern& pattern) {
  return std::string(pattern.str());
}

#endif  // !STOCKHOLM_PATTERN_HPP
