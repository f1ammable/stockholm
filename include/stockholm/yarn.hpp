#ifndef STOCKHOLM_YARN_HPP
#define STOCKHOLM_YARN_HPP
// TODO: Implement dynamic resizing
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>

#include "stockholm/util.hpp"

namespace stockholm::detail {
// 64 seems like a sane maximum for a regex pattern
template <size_t N = 64>
class Yarn {
 private:
  std::array<char, N> m_buffer{};
  size_t m_size;

 public:
  constexpr Yarn() : m_buffer(), m_size(0) {}
  explicit constexpr Yarn(const std::string_view str) : m_buffer(), m_size(0) {
    append(str);
  }
  explicit constexpr Yarn(char c) : m_buffer(), m_size(0) { append(c); }

  template <size_t L>
  explicit constexpr Yarn(const char (&str)[L]) : m_buffer(), m_size(0) {
    append(std::string_view(str, L - 1));
  }

  constexpr Yarn(const Yarn &other) : m_buffer(), m_size(other.m_size) {
    for (size_t i = 0; i < m_size; i++) {
      m_buffer[i] = other.m_buffer[i];
    }
  }

  template <size_t M>
  constexpr Yarn(const std::array<char, M> &buf) : m_buffer(), m_size(0) {
    append(std::string_view(buf.data(), std::min(M, N - 1)));
  }

  constexpr void append(const std::string_view str) {
    for (char c : str) {
      if (m_size < N - 1) {
        m_buffer[m_size++] = c;
      }
    }
    m_buffer[m_size] = '\0';
  }

  constexpr void append(char c) {
    if (m_size < N - 1) {
      m_buffer[m_size++] = c;
      m_buffer[m_size] = '\0';
    }
  }

  constexpr void append(const Yarn<> &yarn) { append(yarn.view()); }

  [[nodiscard]] constexpr std::string_view view() const {
    return std::string_view{m_buffer.data(), m_size};
  }

  [[nodiscard]] constexpr const char *c_str() const { return m_buffer.data(); }

  [[nodiscard]] constexpr size_t size() const { return m_size; }

  constexpr Yarn operator+(const Yarn &other) const {
    Yarn result = *this;
    result.append(other.view());
    return result;
  }

  constexpr const char &operator[](size_t idx) const { return m_buffer[idx]; }

  constexpr char &operator[](size_t idx) { return m_buffer[idx]; }

  constexpr void pop_back() { m_size--; }
};

constexpr Yarn<> int_to_yarn(std::integral auto value) {
  if (value == 0) return Yarn<>("0");

  bool negative = value < 0;
  if (negative) value = -value;
  std::array<char, 10> buf{};
  int pos = 9;

  while (value > 0) {
    buf[pos--] = '0' + (value % 10);
    value /= 10;
  }

  if (negative) buf[pos--] = '-';

  return Yarn<>(std::string_view(buf.data() + pos + 1, 9 - pos));
}

template <size_t P = 15>
constexpr auto float_to_yarn(std::floating_point auto value) {
  if (value == 0.0) return Yarn<>("0");

  bool neg = value < 0;
  if (neg) value = -value;

  long long int_part = static_cast<long long>(value);
  double frac_part = value - int_part;

  auto int_yarn = int_to_yarn(neg ? -int_part : int_part);

  if (frac_part < 1e-15) {
    return int_yarn;
  }

  Yarn<32> frac_buf{};
  int pos = 31;

  for (size_t i = 0; i < P; i++) {
    frac_part *= 10;
    int digit = static_cast<int>(frac_part);
    frac_buf.append('0' + digit);
    pos--;
    frac_part -= digit;

    if (frac_part < 1e-15) break;
  }

  while (frac_buf.size() > 0 && frac_buf[frac_buf.size() - 1] == '0') {
    frac_buf.pop_back();
  }

  if (frac_buf.size() > 0) {
    Yarn<> res{};

    for (size_t i = 0; i < int_yarn.size(); i++) {
      res.append(int_yarn[i]);
    }

    res.append('.');

    for (size_t i = 0; i < frac_buf.size(); i++) {
      res.append(frac_buf[i]);
    }

    return res;
  }
}
};  // namespace stockholm::detail

#endif  // !STOCKHOLM_YARN_HPP
