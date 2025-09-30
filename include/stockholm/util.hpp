#ifndef STOCKHOLM_UTIL_HPP
#define STOCKHOLM_UTIL_HPP

#include <string_view>

namespace stockholm::detail {
template <typename T>
concept StrLike = std::convertible_to<T, std::string_view>;

template <typename... Types>
concept AllStrLike = (StrLike<Types> && ...);

template <typename T>
concept Formattable =
    std::is_arithmetic_v<T> || std::convertible_to<T, std::string_view>;

}  // namespace stockholm::detail

#endif  // !STOCKHOLM_UTIL_HPP
