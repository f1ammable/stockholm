#include <string_view>

#include "yarn.hpp"

#ifndef STOCKHOLM_SELECTOR_HPP
#define STOCKHOLM_SELECTOR_HPP

enum Selector { ALPHA, NUM };

template <Selector T>
class Matcher {
  stockholm::detail::Yarn<> m_pattern;

 public:
  explicit consteval Matcher()
    requires(T == ALPHA)
  {
    m_pattern.append("[a-zA-Z0-9]*");
  }

  [[nodiscard]] consteval std::string_view str() const {
    return m_pattern.view();
  }

  explicit consteval Matcher()
    requires(T == NUM)
  {
    m_pattern.append(R"(\d+)");
  }
};

#endif  // !STOCKHOLM_SELECTOR_HPP
