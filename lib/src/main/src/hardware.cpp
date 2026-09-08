
#include "hardware.hpp"

namespace hardware
{

    // ===== Word =====
    // ===== Constructors =====
    Word::Word() : word(0) {}

    // --- Copy constructor
    Word::Word(const Word &value)
    {
        word = value;
    }

    Word::Word(const Byte upper, const Byte lower)
    {
        hi = upper;
        lo = lower;
    }

    Word::Word(const int value) : word(value & BIT_MASK)
    {
    }

    // ===== Operator overloads =====

    // --- operator = (assignment)
    Word Word::operator=(const Word &value)
    {
        word = value;
        return *this;
    }

    Word Word::operator=(const unsigned int value)
    {
        word = (unsigned short)(value & BIT_MASK);

        return *this;
    }

    // --- operator + (addition)
    Word Word::operator+(const unsigned int value)
    {
//        word = (unsigned short)((word + value) & BIT_MASK);
//
//        return *this;
        Word tmp = (unsigned short)((word + value) & BIT_MASK);

        return tmp;
    }

    Word Word::operator+(const unsigned short value)
    {
        word = word + value;

        return *this;
    }

    Word Word::operator+(const Byte value)
    {
//        word = (unsigned short)((word + value) & BIT_MASK);
        Word tmp = (unsigned short)((word + value) & BIT_MASK);

        return tmp;
    }
    Word Word::operator+(const Word value)
    {
        Word tmp = (word + value.word) & BIT_MASK;

        return tmp;
//        word = word + (unsigned short)value;
//
//        return *this;
    }

    // --- operator ++ (increment)
    Word Word::operator++()
    {
        ++word;

        return *this;
    }
    // Word operator++(int)
    // {
    //     ++word;

    //     return *this;
    // }

    // --- operator -- (decrement)
    Word Word::operator--()
    {
        --word;

        return *this;
    }

    // --- operator += (addition assignment)
    Word &Word::operator+=(const Word rhs)
    {
        word += rhs;

        return *this;
    }
    Word &Word::operator+=(const int rhs)
    {
        word += rhs;
        word &= BIT_MASK;

        return *this;
    }

    // --- operator -= (subtraction assignment)
    Word &Word::operator-=(const int rhs)
    {
        word -= rhs;
        word &= BIT_MASK;

        return *this;
    }

    // --- operator () (type conversion)
    Word::operator unsigned short() const
    {
        return word;
    }

    // ===== Address =====
    // ===== Constructors =====
    Address::Address()
    {
    }

    Address::Address(const Byte upper, const Byte lower)
    {
        pch = upper;
        pcl = lower;
    }

    Address::Address(const int vector)
    {
        address = vector & BIT_MASK;
    }
    // Address( const m6502::HardwareVector vector )
    // {

    // }

    Address::Address(const hardware::Byte zeroPage)
    {
        pch = 0;
        pcl = zeroPage;
    }

    // --- Copy constructor
    Address::Address(const Address &value)
    {
        address = value.address;
    }

    Address::Address(const unsigned int value)
    {
        address = value & 0xFFFF;
    }

    Address::Address(const Word value)
    {
        address = value & 0xFFFF;
    }

    // ===== Operator overloads =====

    // --- operator = (assignment)
    Address &Address::operator=(const Address &rhs)
    {
        if (this != &rhs)
        {
            address = rhs.address;
        }

        return *this;
    }
    // Word operator=(const Word value)
    // {
    //     address = value;
    //     return address;
    // }

    // --- operator + (addition)
    Address Address::operator+(const Address value)
    {
        *this += value;

        return *this;
    }
    Address Address::operator+(const Word value)
    {
        address += value;

        return *this;
    }
    Address Address::operator+(const Byte value)
    {
        address = address + value;

        return *this;
    }
    Address Address::operator+(const int value)
    {
        Address tmp = *this;

        tmp += value;

        return tmp;
    }

    // --- operator - (subtraction)

    // --- operator ++ (increment)
    Address &Address::operator++() // prefix operator
    {
        ++address;

        return *this;
    }

    Address &Address::operator++(int) // prefix operator
    {
        ++address;

        return *this;
    }

    // --- operator -- (decrement)
    Address &Address::operator--() // prefix operator
    {
        --address;

        return *this;
    }
    Address &Address::operator--(int) // prefix operator
    {
        --address;

        return *this;
    }

    // Address& operator+=( const Address& rhs) // prefix operator
    // {
    //     ++address;

    //     return *this;
    // }

    // --- operator += (addition assignment)
    Address Address::operator+=(Address &rhs)
    {
        address += rhs.address;

        return *this;
    }
    Address Address::operator+=(const Address &rhs)
    {
        address += rhs.address;

        return *this;
    }
    Address Address::operator+=(const Word &rhs)
    {
        address += rhs;

        return *this;
    }
    Address Address::operator+=(const int rhs)
    {
        address += rhs;

        return *this;
    }

    // --- operator -= (subtraction assignment)
    Address Address::operator-=(const int rhs)
    {
        address -= rhs;

        return *this;
    }

    // --- operator & (bitwise AND)
    Address Address::operator&(const Address &rhs)
    {
        address = address & rhs.address;

        return *this;
    }
    // Address operator&(const Word &rhs)
    // {
    //     address = address & rhs;

    //     return *this;
    // }

    // --- operator &= (and assignment)
    Address Address::operator&=(const Address &rhs)
    {
        address.word &= rhs.address.word;

        return *this;
    }

    // --- operator |= (or assignment)
    Address Address::operator|=(const Address &rhs)
    {
        address.word |= rhs.address.word;

        return *this;
    }

    // --- operator int() (conversion)
    Address::operator int() const
    {
        return this->address;
    }
};

// Address Address::operator+(int val, const Address &addr);

// --- operator - (subtraction)
// Word operator-(const Address &lhs, const Address &rhs)
// {
//     (int)(lhs.address) - (int)(rhs.address);
// }

// TODO create output stream operator to output the address in hex
//    std::ostream &operator<<(std::ostream &os, const Address &rhs)
//    {
//        // std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(registers.PC) << " Program Counter" << std::endl;
//
//        os << std::setfill('0') << std::setw(4) << rhs.address;
//
//        return os;
//    }

//    std::ostream &operator<<(std::ostream &os, const Word &rhs)
//    {
//        os << rhs;
//
//        return os;
//    }

// Size of the address bus and correspondingly the Program Counter and stack pointer
// using Address1 = unsigned short; // or uint16_t
// using Address = AddressRegister;
// }

hardware::Address hardware::operator+(int val, const hardware::Address &addr)
{
    hardware::Address a = addr.address + val;

    return a;
}
