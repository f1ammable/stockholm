#ifndef STOCKHOLM_SELECTOR_HPP
#define STOCKHOLM_SELECTOR_HPP

#include <string_view>

#include "yarn.hpp"

enum class Selector {
  ALPHANUM,
  ALPHANUM_LOWER,
  ALPHANUM_HIGHER,
  DIGIT,
  LETTER,
  WORD,
  WHITESPACE
};

class Matcher {
  stockholm::detail::Yarn<> m_pattern;

 public:
  explicit consteval Matcher(const Selector s) {
    switch (s) {
      case Selector::ALPHANUM:
        m_pattern.append("[a-zA-Z0-9]");
        break;
      case Selector::ALPHANUM_LOWER:
        m_pattern.append("[a-z0-9]");
        break;
      case Selector::ALPHANUM_HIGHER:
        m_pattern.append("[A-Z0-9]");
        break;
      case Selector::DIGIT:
        m_pattern.append(R"(\d)");
        break;
      case Selector::LETTER:
        m_pattern.append("[a-zA-Z]");
        break;
      case Selector::WORD:
        m_pattern.append(R"(\w)");
        break;
      case Selector::WHITESPACE:
        m_pattern.append(R"(\s)");
        break;
    }
  }

  [[nodiscard]] consteval operator std::string_view() const {
    return m_pattern.view();
  }
};

#endif  // !STOCKHOLM_SELECTOR_HPP
