#include <iostream>

#include "include/pattern.hpp"

int main(int argc, char *argv[]) {
  constexpr auto x = Capture(Start()
                                 .OneOrMore(Matcher(Selector::ALPHANUM))
                                 .One(Matcher(Selector::NUM))
                                 .OneOrMore("b"));
  std::cout << x << std::endl;
  return 0;
}
