#ifndef INCLUDE_FLAGFIELD_H_
#define INCLUDE_FLAGFIELD_H_

#include <bitset>
#include <iostream>
namespace git {

template <typename T, std::size_t N>
class FlagField {
    public:
    using valueType = T;
        FlagField(std::uint32_t value) : m_value(value) {
            std::cout << "FlagField: " << m_value.to_string() << std::endl;
        }

        bool operator[](T flag) const {
            std::cout << "operator[]: " << m_value.to_string() << std::endl;
            std::cout << "flag: " << (static_cast<std::size_t>(flag)>>1) << std::endl;
            return m_value.test(static_cast<std::size_t>(flag)>>1);
        }

        bool at(std::uint32_t index) const {
            return m_value.test(index);
        }

    private:
        std::bitset<N> m_value;
};

}  // namespace git

#endif  // INCLUDE_FLAGFIELD_H_