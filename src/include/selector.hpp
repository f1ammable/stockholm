#include <string_view>

#include "yarn.hpp"

#ifndef STOCKHOLM_SELECTOR_HPP
#define STOCKHOLM_SELECTOR_HPP

enum class Selector { ALPHA, NUM };

class Matcher {
  stockholm::detail::Yarn<> m_pattern;

 public:
  explicit consteval Matcher(Selector s) {
    switch (s) {
      case Selector::ALPHA:
        m_pattern.append("a-zA-Z0-9");
        break;
      case Selector::NUM:
        m_pattern.append(R"(\d+)");
        break;
    }
  }

  [[nodiscard]] consteval operator std::string_view() const {
    return m_pattern.view();
  }
};

#endif  // !STOCKHOLM_SELECTOR_HPP
