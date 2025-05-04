#include <iostream>

#include "include/pattern.hpp"

int main(int argc, char *argv[]) {
  constexpr auto x = Capture(Group(OneOrMore("b")).Group(OneOrMore("@")));
  std::cout << x << std::endl;
  return 0;
}
