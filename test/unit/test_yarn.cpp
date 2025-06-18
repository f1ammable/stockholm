#include <catch2/catch_test_macros.hpp>
#include <stockholm/yarn.hpp>

using namespace stockholm::detail;

TEST_CASE("Yarn - Construction", "[yarn] [construction]") {
  SECTION("Default constructor creates empty yarn") {
    const Yarn<> yarn;
    CHECK(yarn.size() == 0);
    CHECK(yarn.view().empty());
    CHECK(yarn.c_str()[0] == '\0');
  }

  SECTION("String view constructor") {
    const Yarn yarn(std::string_view("test"));
    CHECK(yarn.size() == 4);
    CHECK(yarn.view() == "test");
    CHECK(std::string_view(yarn.c_str()) == "test");
  }

  SECTION("Character constructor") {
    Yarn yarn('A');
    CHECK(yarn.size() == 1);
    CHECK(yarn.view() == "A");
    CHECK(yarn.c_str()[0] == 'A');
    CHECK(yarn.c_str()[1] == '\0');
  }

  SECTION("Character array constructor") {
    Yarn yarn("literal");
    CHECK(yarn.size() == 7);
    CHECK(yarn.view() == "literal");
    CHECK(std::string_view(yarn.c_str()) == "literal");
  }

  SECTION("Copy constructor") {
    Yarn original("original");
    Yarn copy(original);
    CHECK(copy.size() == original.size());
    CHECK(copy.view() == original.view());
    CHECK(copy.view() == "original");
  }
}

TEST_CASE("Yarn - Template Parameters", "[yarn] [template]") {
  SECTION("Capacity limits are respected") {
    const Yarn<5> yarn("123456789");
    CHECK(yarn.size() == 4);
    CHECK(yarn.view().length() == 4);
  }

  // SECTION("Dynamic resizing")
}

TEST_CASE("Yarn - Append Operations", "[yarn] [append]") {
  SECTION("Append string_view") {
    Yarn yarn("start");
    yarn.append(" middle");
    yarn.append(" end");

    CHECK(yarn.view() == "start middle end");
    CHECK(yarn.size() == 16);
  }

  SECTION("Append empty string") {
    Yarn yarn("test");
    const size_t original_size = yarn.size();
    yarn.append(std::string_view(""));
    CHECK(yarn.size() == original_size);
    CHECK(yarn.view() == "test");
  }

  SECTION("Append respects capacity limits") {
    Yarn<10> small("start");
    small.append("verylongstring");
    CHECK(small.size() <= 9);
    CHECK(small.c_str()[small.size()] == '\0');
  }
}

TEST_CASE("Yarn - Accessors", "[yarn] [accessors]") {
  SECTION("view() returns correct string_view") {
    const Yarn yarn("start");
    auto view = yarn.view();
    CHECK(view.size() == 5);
    CHECK(view.data() == yarn.c_str());
    CHECK(view == "start");
  }

  SECTION("c_str() returns null-terminated string") {
    Yarn yarn("test_cstr");
    const char* cstr = yarn.c_str();
    CHECK(std::string(cstr) == "test_cstr");
    CHECK(cstr[yarn.size()] == '\0');
  }

  SECTION("size() returns correct length") {
    Yarn yarn;
    CHECK(yarn.size() == 0);

    yarn.append("test");
    CHECK(yarn.size() == 4);
  }
}

TEST_CASE("Yarn - Operators", "[yarn] [operators]") {
  SECTION("Basic concatenation") {
    Yarn a("foo");
    Yarn b("bar");
    const auto result = a + b;
    CHECK(result.view() == "foobar");
    CHECK(result.size() == 6);
  }

  SECTION("Concatenation with empty yarns") {
    const Yarn empty;
    const Yarn content("test");
    CHECK((empty + content).view() == "test");
    CHECK((content + empty).view() == "test");
  }

  SECTION("Chaining multiple concatenations") {
    Yarn one("1");
    Yarn two("2");
    Yarn three("3");
    auto result = one + two + three;
    CHECK(result.view() == "123");
    CHECK(result.size() == 3);
  }

  SECTION("Original yarns remain unchanged after concatenation") {
    Yarn original("original");
    Yarn other("other");

    auto combined = original + other;
    CHECK(original.view() == "original");
    CHECK(other.view() == "other");
    CHECK(combined.view() == "originalother");
  }
}

TEST_CASE("Yarn - Compile-time Evaluation", "[yarn] [constexpr]") {
  constexpr auto test = [] {
    Yarn yarn("start");
    yarn.append(" middle");
    yarn.append('!');

    const Yarn other(" end");
    const auto combined = yarn + other;
    return combined.view() == "start middle! end" && combined.size() == 17;
  };

  static_assert(test());
}