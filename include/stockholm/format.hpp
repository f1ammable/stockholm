#ifndef STOCKHOLM_FORMAT_HPP
#define STOCKHOLM_FORMAT_HPP

#include <concepts>
#include <numeric>
#include <string_view>
#include <type_traits>

#include "stockholm/util.hpp"
#include "yarn.hpp"

namespace stockholm::detail {

template <size_t N, size_t Count>
struct compiled_format {
  std::array<char, N> buf;
  static constexpr size_t count = Count;
  static constexpr size_t size = N;

  constexpr compiled_format(const char (&str)[N]) {
    for (size_t i = 0; i < N; i++) buf[i] = str[i];
  }

  constexpr std::string_view view() const {
    return std::string_view(buf.data(), N - 1);
  }

  constexpr char& operator[](size_t idx) { return this->buf[idx]; }

  constexpr const char& operator[](size_t idx) const { return this->buf[idx]; }
};
}  // namespace stockholm::detail

#define COMPILE_FORMAT(str)                                   \
  ([]() constexpr {                                           \
    return stockholm::detail::compiled_format<                 \
        sizeof(str), stockholm::detail::count_fmt(str)>{str}; \
  }())

namespace stockholm::detail {

// template <size_t N>
// compiled_format(const char (&str)[N]) -> compiled_format<N, count_fmt(str)>;

template <typename T>
struct is_compiled_format : std::false_type {};

template <size_t N, size_t Count>
struct is_compiled_format<compiled_format<N, Count>> : std::true_type {};

template <typename T>
concept CompiledFormat = is_compiled_format<T>::value;

template <size_t N>
constexpr size_t count_fmt(const char (&format)[N]) {
  size_t c = 0;
  for (size_t i = 0; i < N - 1; i++) {
    if (format[i] == '{' && format[i + 1] == '}') {
      c++;
      i++;
    }
  }
  return c;
}

template <Formattable T>
constexpr size_t len(const T& str) {
  if constexpr (std::same_as<std::decay_t<T>, std::string_view>) {
    return str.length();
  } else {
    return std::string::traits_type::length(str);
  }
}

template <size_t N, StrLike... T>
constexpr std::array<size_t, N + 1> total_len(const T&... strs) {
  std::array<size_t, N + 1> res{};
  size_t idx = 0;

  ((res[idx++] = len(strs)), ...);
  res[N] = std::accumulate(res.begin(), res.begin() + N, size_t{0});

  return res;
}

constexpr Yarn<> bool_to_yarn(bool value) {
  if (value == true) {
    return Yarn<>("true");
  }
  return Yarn<>("false");
}

template <Formattable T>
constexpr Yarn<> to_yarn(const T& value) {
  if constexpr (std::same_as<std::decay_t<T>, char>) {
    return Yarn<>(value);
  } else if constexpr (std::same_as<std::remove_cvref_t<T>, bool>) {
    return bool_to_yarn(value);
  } else if constexpr (std::is_integral_v<T>) {
    return int_to_yarn(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    return float_to_yarn(value);
  }
  return Yarn<>(value);
}

template <CompiledFormat Format, Formattable... Args>
constexpr Yarn<> format(const Format& format, const Args&... args) {
  static_assert(sizeof...(Args) == Format::count,
                "Mismatch between number of arguments and format specifiers");

  const std::array<Yarn<>, sizeof...(Args)> arg_list{to_yarn(args)...};
  auto res = Yarn<>();
  size_t arg_idx = 0;
  size_t res_idx = 0;

  for (int i = 0; i < Format::size; i++) {
    if (format[i] == '{' && format[i + 1] == '}') {
      for (int j = 0; j < arg_list[arg_idx].size(); j++) {
        res.append(arg_list[arg_idx][j]);
        res_idx++;
      }
      arg_idx++;
      i++;
    } else {
      res.append(format[i]);
      res_idx++;
    }
  }

  return res;
}
};  // namespace stockholm::detail

#endif
