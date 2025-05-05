#include <iostream>

#include "include/pattern.hpp"

int main(int argc, char *argv[]) {
  constexpr auto x = Capture(OneOrMore(Matcher<ALPHA>()));
  std::cout << x << std::endl;
  return 0;
}
