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
A basic demonstration can be seen [here](https://github.com/f1ammable/stockholm/blob/main/src/main.cpp).

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

