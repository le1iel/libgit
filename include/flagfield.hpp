#ifndef INCLUDE_FLAGFIELD_HPP_
#define INCLUDE_FLAGFIELD_HPP_

#include <bitset>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ostream>
#include <string>
#include <type_traits>
#include <cstddef>
#include <initializer_list>

constexpr std::size_t FLAGFIELD_DEFAULT_SIZE = 32;

namespace gitxx {

/// @brief std::bitset that can be typed using an enum.
template <typename T, std::size_t Size = FLAGFIELD_DEFAULT_SIZE,
          typename = std::enable_if_t<std::is_enum_v<T>>>
class FlagField {
 public:
  /// @brief The internal value type.
  using ValueType = T;

  /// @brief Constructor from a value.
  constexpr explicit FlagField(std::uint32_t value) : m_value(value) {}

  /// @brief Constructor from a initializer list.
  constexpr FlagField(std::initializer_list<T> enum_list)
      : m_value(std::accumulate(
            enum_list.begin(), enum_list.end(), 0,
            [](std::uint32_t value, T flag) {
              return (value | 1U << static_cast<std::uint32_t>(flag));
            })) {}

  /// @brief Constructor from a value.
  constexpr explicit FlagField(ValueType value) : m_value(0) { set(value); }

  /// @brief Constructor from a value.
  constexpr FlagField() = default;

  /// @brief Check if the given enum is set.
  [[nodiscard]] constexpr bool test(ValueType flag) const {
    return m_value.test(static_cast<std::size_t>(flag));
  }

  /// @brief Check if the given enum is set.
  [[nodiscard]] constexpr bool operator[](ValueType flag) const {
    return test(flag);
  }

  /// @brief Returns a reference to the bit at the given index which you
  /// can mutate.
  constexpr std::bitset<Size>::reference operator[](ValueType flag) {
    return m_value[static_cast<uint32_t>(flag)];
  }

  /// @brief Set a flag.
  constexpr void set(ValueType flag) {
    m_value.set(static_cast<std::size_t>(flag));
  }

  /// @brief Check if no flag is set.
  [[nodiscard]] constexpr bool none() const { return m_value.none(); }

  /// @brief Check if any flag is set.
  [[nodiscard]] constexpr bool any() const { return m_value.any(); }

  /// @brief Get the value of the flag field.
  [[nodiscard]] constexpr std::uint32_t value() const {
    return static_cast<std::uint32_t>(m_value.to_ulong());
  }

  /// @brief Get string representation of the bitset.
  [[nodiscard]] std::string to_string() const { return m_value.to_string(); }

 private:
  /// @brief The value of the flag field.
  std::bitset<Size> m_value{0U};
};

/// @brief Stream insertion operator for FlagField.
template <typename T, std::size_t Size, typename E>
[[nodiscard]] std::ostream& operator<<(std::ostream& outstream,
                                       const FlagField<T, Size, E>& flags) {
  return outstream << flags.to_string();
}

}  // namespace gitxx

#endif  // INCLUDE_FLAGFIELD_HPP_
