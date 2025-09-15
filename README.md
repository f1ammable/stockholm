## Stockholm
A header-only C++ library heavily inspired by Swift's RegexBuilder.

## Overview
Stockholm offers a declarative and readable approach to constructing regular expressions, making your regex code more maintanable and self-documenting.

## Current Status
This is an early-stage project and currently implements very basic functionality. It is currently structured as an executable for testing purposes rather than a packaged library. Expect breaking changes.

## (Planned) Features
- [x] Basic pattern building blocks
- [ ] Quantifiers
- [ ] Capture groups and references
- [ ] Comprehensive test suite
- [ ] Package as header-only library
- [ ] Documentation
- [ ] Integration with ctre 

## Requirements
- C++20 compatible compiler
- CMake 3.30 or newer

## Usage
Here is a basic example of what the library currently has to offer

```cpp
#include <iostream>
#include <stockholm.hpp>

#include "stockholm/pattern.hpp"
#include "stockholm/selector.hpp"

int main() {
  // Basic email pattern
  constexpr std::string pattern =
      Capture(stockholm::Start()
                  .OneOrMore(Selector::ALPHANUM)
                  .One("@")
                  .OneOrMore(Selector::ALPHANUM)
                  .One(".")
                  .Quantify(Selector::ALPHANUM, 2, 4));

  std::cout << pattern << std::endl;
}
```

## Building
This project uses CMake as its build system:

```bash
# Clone
git clone https://github.com/f1ammable/stockholm.git
cd stockholm

# Create build directory
mkdir build
cd build

# Compile
cmake .. --preset release
make

# Run demo
./stockholm
```

