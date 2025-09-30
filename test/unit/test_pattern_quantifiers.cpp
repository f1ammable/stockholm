#include <catch2/catch_test_macros.hpp>
#include <stockholm.hpp>

TEST_CASE("Pattern - Quantifiers", "[pattern] [quantifiers]") {
  SECTION("Exactly() creates correct quantifier") {
    constexpr static auto pattern = stockholm::Start().Exactly("abc", 3);
    constexpr auto res = std::string(pattern.str());
    CHECK(res.find("abc{3}") != std::string::npos);
  }

  SECTION("AtLeat() creates minimum quantifier") {
    constexpr static auto pattern = stockholm::Start().AtLeast("abc", 3);
    constexpr auto res = std::string(pattern.str());
    CHECK(res.find("abc{3,}") != std::string::npos);
  }

  SECTION("AtMost() creates maximum quantifier") {
    constexpr static auto pattern = stockholm::Start().AtMost("abc", 3);
    constexpr auto res = std::string(pattern.str());
    CHECK(res.find("abc{,3}") != std::string::npos);
  }

  SECTION("Quantify() creates range quantifier") {
    constexpr static auto pattern = stockholm::Start().Quantify("abc", 1, 3);
    constexpr auto res = std::string(pattern.str());
    CHECK(res.find("abc{1,3}") != std::string::npos);
  }
}
