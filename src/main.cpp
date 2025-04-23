#include "include/pattern.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  constexpr auto x = Capture(OneOrMore("b"));
  std::cout << x << std::endl;
  return 0;
}
