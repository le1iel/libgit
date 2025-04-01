#ifndef INCLUDE_FLAGFIELD_H_
#define INCLUDE_FLAGFIELD_H_

#include <bitset>
#include <cstdint>
#include <iostream>

namespace git {

template <typename T, std::size_t N>
class FlagField {
    public:
        using ValueType = T;

        FlagField(std::uint32_t value) : m_value(value) {
        }

        FlagField& operator=(FlagField&& other) {
            m_value = other.m_value;
            other.m_value = 0;
            return *this;
        }

        bool operator[](ValueType flag) const {
            return m_value.test(static_cast<std::size_t>(flag));
        }

        bool at(std::uint32_t index) const {
            return m_value.test(index);
        }

        void set(ValueType flag) {
            m_value.set(static_cast<std::size_t>(flag)>>1);
        }

        void set(std::uint32_t index) {
            m_value (index);
        }

        std::string to_string() const {
            return m_value.to_string();
        }

    private:
        std::bitset<N> m_value;
};

}  // namespace git

#endif  // INCLUDE_FLAGFIELD_H_