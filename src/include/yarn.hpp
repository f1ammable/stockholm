#ifndef STOCKHOLM_YARN_HPP
#define STOCKHOLM_YARN_HPP

#include <cstddef>
#include <string_view>

namespace stockholm::detail {
// 64 seems like a sane maximum for a regex pattern
template <size_t capacity = 64>
class Yarn {
 private:
  char m_buffer[capacity];
  size_t m_size;

 public:
  inline explicit consteval Yarn() : m_buffer(), m_size(0) {}
  inline explicit consteval Yarn(std::string_view str) : m_buffer(), m_size(0) {
    append(str);
  }
  inline explicit consteval Yarn(char c) : m_buffer(), m_size(0) { append(c); }

  template <std::size_t N>
  inline explicit consteval Yarn(const char (&str)[N]) : m_buffer(), m_size(0) {
    append(std::string_view(str, N - 1));
  }

  inline consteval Yarn(const Yarn &other) : m_size(other.m_size) {
    for (size_t i = 0; i < m_size; i++) {
      m_buffer[i] = other.m_buffer[i];
    }
  }

  inline consteval void append(std::string_view str) {
    for (char c : str) {
      if (m_size < capacity - 1) {
        m_buffer[m_size++] = c;
      }
    }
    m_buffer[m_size] = '\0';
  }

  inline consteval void append(char c) {
    if (m_size < capacity - 1) {
      m_buffer[m_size++] = c;
      m_buffer[m_size] = '\0';
    }
  }

  inline consteval std::string_view view() const {
    return std::string_view(m_buffer, m_size);
  }

  inline consteval const char *c_str() const { return m_buffer; }

  inline consteval size_t size() const { return m_size; }
};
}  // namespace stockholm::detail

#endif  // !STOCKHOLM_YARN_HPP
