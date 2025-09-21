#ifndef STOCKHOLM_FORMAT_HPP
#define STOCKHOLM_FORMAT_HPP

#include <concepts>
#include <numeric>
#include <string_view>

#include "util.hpp"
#include "yarn.hpp"

namespace stockholm::detail {

template <size_t N>
consteval int count_fmt(char const (&format)[N]) {
  int c = 0;
  for (size_t i = 0; i < N - 1; i++) {
    if (format[i] == '{' && format[i + 1] == '}') {
      c++;
      i++;
    }
  }
  return c;
}

template <Formattable T>
consteval size_t len(const T& str) {
  return std::string::traits_type::length(str);
}

template <size_t N, StrLike... T>
consteval std::array<size_t, N + 1> total_len(const T&... strs) {
  std::array<size_t, N + 1> res{};
  size_t idx = 0;

  ((res[idx++] = len(strs)), ...);
  res[N] = std::accumulate(res.begin(), res.begin() + N, size_t{0});

  return res;
}

consteval Yarn<10> int_to_yarn(std::integral auto value) {
  if (value == 0) return Yarn<10>("0");

  bool negative = value < 0;
  if (negative) value = -value;
  std::array<char, 10> buf{};
  int pos = 9;

  while (value > 0) {
    buf[pos--] = '0' + (value % 10);
    value /= 10;
  }

  if (negative) buf[pos--] = '-';

  return Yarn<10>(std::string_view(buf.data() + pos + 1, 9 - pos));
}

template <Formattable T>
consteval Yarn<> to_yarn(const T& value) {
  if constexpr (std::integral<T>) {
    return int_to_yarn(value);
  } else if constexpr (std::same_as<std::decay_t<T>, char>) {
    return Yarn<>(value);
  }
  return Yarn<>(value);
}

template <size_t N, Formattable... T>
consteval auto format(char const (&format)[N], const T&... args) {
  static_assert(sizeof...(args) == count_fmt(format),
                "Mismatch between number of arguments and format specifiers");

  auto lens = total_len<sizeof...(args)>(args...);
  std::array<Yarn<>, sizeof...(args)> arg_list{to_yarn(args)...};
  auto res = Yarn<N + lens.back()>();
  size_t arg_idx = 0;
  size_t res_idx = 0;

  for (int i = 0; i < N - 1; i++) {
    if (format[i] == '{' && format[i + 1] == '}') {
      for (int j = 0; j < arg_list[arg_idx].size(); j++) {
        res[res_idx] = arg_list[arg_idx][j];
        res_idx++;
      }
      arg_idx++;
      i++;
    } else {
      res[res_idx] = format[i];
      res_idx++;
    }
  }

  return res;
}
};  // namespace stockholm::detail

#endif
