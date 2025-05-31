#include <iostream>

#include "include/pattern.hpp"

int main(int argc, char *argv[]) {
  constexpr auto x =
      Capture(OneOrMore(Matcher(Selector::ALPHA)).OneOrMore("b"));
  std::cout << x << std::endl;
  return 0;
}
