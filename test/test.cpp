#include <catch2/catch_test_macros.hpp>
#include <stockholm.hpp>

TEST_CASE("Pattern - Basic Construction", "[pattern] [basic]") {
  SECTION("Start creates empty pattern") {
    constexpr static auto pattern = stockholm::Start();
    CHECK(pattern.str().empty());
  }

  SECTION("String view returns valid content") {
    constexpr static auto pattern = stockholm::Start();
    constexpr auto view = pattern.str();
    CHECK(view.data() != nullptr);
  }
}

TEST_CASE("Pattern - Basic Elements", "[pattern] [elements]") {
  SECTION("One() wraps content correctly") {
    constexpr static auto pattern = stockholm::Start().One("abc");
    constexpr auto res = std::string(pattern.str());
    CHECK(res.find("abc") != std::string::npos);
  }
}