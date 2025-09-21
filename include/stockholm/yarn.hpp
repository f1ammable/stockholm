#ifndef STOCKHOLM_YARN_HPP
#define STOCKHOLM_YARN_HPP
// TODO: Implement dynamic resizing
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <string_view>

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
};
};  // namespace stockholm::detail

#endif  // !STOCKHOLM_YARN_HPP
