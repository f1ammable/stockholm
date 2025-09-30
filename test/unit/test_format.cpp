#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <stockholm/format.hpp>
#include <stockholm/yarn.hpp>
#include <string_view>

using namespace stockholm::detail;

template <size_t N>
bool yarn_equals(const Yarn<N>& yarn, std::string_view expected) {
  // +1 because string_view size doesn't include null terminator
  if (yarn.size() != (expected.size() + 1)) return false;
  for (size_t i = 0; i < yarn.size(); i++) {
    if (yarn[i] != expected[i]) return false;
  }
  return true;
}

TEST_CASE("format function basic functionality", "[format]") {
  SECTION("single string argument") {
    constexpr auto result = format(COMPILE_FORMAT("Hello {}!"), "world");
    REQUIRE(yarn_equals(result, "Hello world!"));
  }

  SECTION("multiple string arguments") {
    constexpr auto result =
        format(COMPILE_FORMAT("Hello {} and {}!"), "Alice", "Bob");
    REQUIRE(yarn_equals(result, "Hello Alice and Bob!"));
  }

  SECTION("no arguments - plain string") {
    constexpr auto result = format(COMPILE_FORMAT("Just a plain string"));
    REQUIRE(yarn_equals(result, "Just a plain string"));
  }
}

TEST_CASE("format function with numeric types", "[format][numeric]") {
  SECTION("integer argument") {
    constexpr auto result = format(COMPILE_FORMAT("Number: {}"), 42);
    REQUIRE(yarn_equals(result, "Number: 42"));
  }

  SECTION("negative integer") {
    constexpr auto result = format(COMPILE_FORMAT("Negative: {}"), -123);
    REQUIRE(yarn_equals(result, "Negative: -123"));
  }

  SECTION("floating point number") {
    auto result = format(COMPILE_FORMAT("Float: {}"), 3.14);
    REQUIRE(yarn_equals(result, "Float: 3.14"));
  }
}

TEST_CASE("format function with mixed types", "[format][mixed]") {
  SECTION("string and number") {
    constexpr auto result =
        format(COMPILE_FORMAT("Name: {}, Age: {}"), "John", 25);
    REQUIRE(yarn_equals(result, "Name: John, Age: 25"));
  }

  SECTION("multiple mixed types") {
    auto result = format(COMPILE_FORMAT("Name: {}, Age: {}, Active: {}"),
                         "John", 25, true);
    REQUIRE(yarn_equals(result, "Name: John, Age: 25, Active: true"));
  }

  SECTION("boolean values") {
    auto result1 = format(COMPILE_FORMAT("True: {}"), true);
    REQUIRE(yarn_equals(result1, "True: true"));

    constexpr auto result2 = format(COMPILE_FORMAT("False: {}"), false);
    REQUIRE(yarn_equals(result2, "False: false"));
  }
}

TEST_CASE("format function placeholder positions", "[format][positions]") {
  SECTION("placeholder at beginning") {
    constexpr auto result = format(COMPILE_FORMAT("{} is the answer"), 42);
    REQUIRE(yarn_equals(result, "42 is the answer"));
  }

  SECTION("placeholder at end") {
    constexpr auto result = format(COMPILE_FORMAT("Result: {}"), "success");
    REQUIRE(yarn_equals(result, "Result: success"));
  }

  SECTION("placeholder in middle") {
    constexpr auto result = format(COMPILE_FORMAT("start {} end"), "middle");
    REQUIRE(yarn_equals(result, "start middle end"));
  }

  SECTION("multiple placeholders at different positions") {
    constexpr auto result =
        format(COMPILE_FORMAT("{} middle {}"), "start", "end");
    REQUIRE(yarn_equals(result, "start middle end"));
  }
}

TEST_CASE("format function consecutive placeholders", "[format][consecutive]") {
  SECTION("two consecutive placeholders") {
    constexpr auto result = format(COMPILE_FORMAT("{}{}"), "A", "B");
    REQUIRE(yarn_equals(result, "AB"));
  }

  SECTION("three consecutive placeholders") {
    constexpr auto result = format(COMPILE_FORMAT("{}{}{}"), "A", "B", "C");
    REQUIRE(yarn_equals(result, "ABC"));
  }

  SECTION("consecutive placeholders with text") {
    constexpr auto result =
        format(COMPILE_FORMAT("Start{}{}{} End"), "A", "B", "C");
    REQUIRE(yarn_equals(result, "StartABC End"));
  }
}

TEST_CASE("format function edge cases", "[format][edge]") {
  SECTION("single character argument") {
    auto result = format(COMPILE_FORMAT("Char: '{}'"), 'X');
    REQUIRE(yarn_equals(result, "Char: 'X'"));
  }

  SECTION("empty string argument") {
    constexpr auto result = format(COMPILE_FORMAT("Empty: '{}'"), "");
    REQUIRE(yarn_equals(result, "Empty: ''"));
  }

  SECTION("zero value") {
    constexpr auto result = format(COMPILE_FORMAT("Zero: {}"), 0);
    REQUIRE(yarn_equals(result, "Zero: 0"));
  }

  // TODO: Make Yarn "dynamically resizeable" first and then come back to this
  //
  // SECTION("very long string") {
  //     constexpr auto long_str = "This is a very long string that should test
  //     the capacity handling"; constexpr auto result =
  //     format(COMPILE_FORMAT("Long: {}"), long_str);
  //     REQUIRE(yarn_equals(result, "Long: This is a very long string that
  //     should test the capacity handling"));
  // }
}

TEST_CASE("format function literal braces", "[format][braces]") {
  SECTION("format with literal single braces") {
    // Assuming single braces that don't form {} are treated as literals
    constexpr auto result =
        format(COMPILE_FORMAT("Value: {} (not a placeholder: {x})"), "test");
    REQUIRE(yarn_equals(result, "Value: test (not a placeholder: {x})"));
  }

  SECTION("format with unmatched braces") {
    constexpr auto result =
        format(COMPILE_FORMAT("Open { and close } with {}"), "value");
    REQUIRE(yarn_equals(result, "Open { and close } with value"));
  }
}

TEST_CASE("format function special characters", "[format][special]") {
  SECTION("newline characters") {
    constexpr auto result = format(COMPILE_FORMAT("Line1\n{}"), "Line2");
    REQUIRE(yarn_equals(result, "Line1\nLine2"));
  }

  SECTION("tab characters") {
    constexpr auto result = format(COMPILE_FORMAT("Before\t{}"), "After");
    REQUIRE(yarn_equals(result, "Before\tAfter"));
  }

  SECTION("quotes and special characters") {
    constexpr auto result = format(COMPILE_FORMAT("Quote: \"{}\""), "text");
    REQUIRE(yarn_equals(result, "Quote: \"text\""));
  }
}

TEST_CASE("format function compile-time argument count validation",
          "[format][compile-time]") {
  SECTION("correct argument count compiles") {
    // These should compile without issues
    constexpr auto result1 = format(COMPILE_FORMAT("Hello {}!"), "world");
    REQUIRE(yarn_equals(result1, "Hello world!"));

    constexpr auto result2 =
        format(COMPILE_FORMAT("Hello {} and {}!"), "Alice", "Bob");
    REQUIRE(yarn_equals(result2, "Hello Alice and Bob!"));

    constexpr auto result3 = format(COMPILE_FORMAT("No placeholders"));
    REQUIRE(yarn_equals(result3, "No placeholders"));
  }

  // Note: The following test cases should cause compilation errors
  // Uncomment individually to verify they fail at compile time:

  /*
  SECTION("too many arguments should not compile") {
      // This should fail to compile due to static_assert:
      // constexpr auto result = format(COMPILE_FORMAT("Hello {}!"), "world",
  "extra");
  }

  SECTION("too few arguments should not compile") {
      // This should fail to compile due to static_assert:
      // constexpr auto result = format(COMPILE_FORMAT("Hello {} and {}!"),
  "world");
  }
  */
}

TEST_CASE("format function performance edge cases", "[format][performance]") {
  SECTION("many arguments") {
    constexpr auto result = format(COMPILE_FORMAT("{} {} {} {} {} {} {} {}"),
                                   "a", "b", "c", "d", "e", "f", "g", "h");
    REQUIRE(yarn_equals(result, "a b c d e f g h"));
  }

  SECTION("alternating text and placeholders") {
    constexpr auto result = format(COMPILE_FORMAT("a{}b{}c{}d"), "1", "2", "3");
    REQUIRE(yarn_equals(result, "a1b2c3d"));
  }
}
