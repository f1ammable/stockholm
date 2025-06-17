#ifndef STOCKHOLM_YARN_HPP
#define STOCKHOLM_YARN_HPP

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
  explicit consteval Yarn() : m_buffer(), m_size(0) {}
  explicit consteval Yarn(const std::string_view str) : m_buffer(), m_size(0) {
    append(str);
  }
  explicit consteval Yarn(char c) : m_buffer(), m_size(0) { append(c); }

  template <std::size_t N>
  explicit consteval Yarn(const char (&str)[N]) : m_buffer(), m_size(0) {
    append(std::string_view(str, N - 1));
  }

  consteval Yarn(const Yarn &other) : m_buffer(), m_size(other.m_size) {
    for (size_t i = 0; i < m_size; i++) {
      m_buffer[i] = other.m_buffer[i];
    }
  }

  consteval void append(const std::string_view str) {
    for (char c : str) {
      if (m_size < capacity - 1) {
        m_buffer[m_size++] = c;
      }
    }
    m_buffer[m_size] = '\0';
  }

  consteval void append(char c) {
    if (m_size < capacity - 1) {
      m_buffer[m_size++] = c;
      m_buffer[m_size] = '\0';
    }
  }

  [[nodiscard]] consteval std::string_view view() const {
    return std::string_view(m_buffer, m_size);
  }

  [[nodiscard]] consteval const char *c_str() const { return m_buffer; }

  [[nodiscard]] consteval size_t size() const { return m_size; }

  consteval Yarn operator+(const Yarn &other) const {
    Yarn result = *this;
    for (size_t i = 0; i < m_size && m_size < capacity - 1; i++) {
      result.m_buffer[i++] = other.m_buffer[i];
    }
    result.m_buffer[m_size] = '\0';
    return result;
  }
};
}  // namespace stockholm::detail

#endif  // !STOCKHOLM_YARN_HPP
