#ifndef INCLUDE_FLAGFIELD_H_
#define INCLUDE_FLAGFIELD_H_

#include <cstdint>

namespace git {

template <typename T>
class FlagField {
    public:
        using ValueType = T;

        /// @brief Constructor from a value.
        FlagField(std::uint32_t value) : m_value(value) { }

        /// @brief Constructor from a value.
        FlagField() = default;

        /// @brief Check if the given enum is set.
        bool test(ValueType flag) const {
            return !!(m_value & (1 << static_cast<std::uint32_t>(flag)));
        }

        /// @brief Check if the given enum is set.
        bool operator[](ValueType flag) const {
            return test(flag);
        }

        /// @brief Set a flag.
        void set(ValueType flag) {
            m_value |= (1 << static_cast<std::uint32_t>(flag));
        }

        /// @brief Check if no flag is set.
        bool none() const {
            return m_value == 0;
        }

        /// @brief Check if any flag is set.
        bool any() const {
            return m_value != 0;
        }

        /// @brief Get the value of the flag field.
        std::uint32_t value() const {
            return m_value;
        }

    private:
        /// @brief The value of the flag field.
        std::uint32_t m_value { 0U };
};

}  // namespace git

#endif  // INCLUDE_FLAGFIELD_H_