#ifndef STOCKHOLM_YARN_HPP
#define STOCKHOLM_YARN_HPP
// TODO: Implement dynamic resizing
#include <cstring>
#include <string_view>

namespace stockholm::detail {
// 64 seems like a sane maximum for a regex pattern
template <size_t capacity = 64>
class Yarn {
 private:
  char m_buffer[capacity];
  size_t m_size;

 public:
  explicit constexpr Yarn() : m_buffer(), m_size(0) {}
  explicit constexpr Yarn(const std::string_view str) : m_buffer(), m_size(0) {
    append(str);
  }
  explicit constexpr Yarn(char c) : m_buffer(), m_size(0) { append(c); }

  template <std::size_t N>
  explicit constexpr Yarn(const char (&str)[N]) : m_buffer(), m_size(0) {
    append(std::string_view(str, N - 1));
  }

  constexpr Yarn(const Yarn &other) : m_buffer(), m_size(other.m_size) {
    for (size_t i = 0; i < m_size; i++) {
      m_buffer[i] = other.m_buffer[i];
    }
  }

  constexpr void append(const std::string_view str) {
    for (char c : str) {
      if (m_size < capacity - 1) {
        m_buffer[m_size++] = c;
      }
    }
    m_buffer[m_size] = '\0';
  }

  constexpr void append(char c) {
    if (m_size < capacity - 1) {
      m_buffer[m_size++] = c;
      m_buffer[m_size] = '\0';
    }
  }

  [[nodiscard]] constexpr std::string_view view() const {
    return std::string_view(m_buffer, m_size);
  }

  [[nodiscard]] constexpr const char *c_str() const { return m_buffer; }

  [[nodiscard]] constexpr size_t size() const { return m_size; }

  constexpr Yarn operator+(const Yarn &other) const {
    Yarn result = *this;
    result.append(other.view());
    return result;
  }
};
}  // namespace stockholm::detail

#endif  // !STOCKHOLM_YARN_HPP
