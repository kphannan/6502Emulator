

#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <iostream>
#include <iomanip>

namespace hardware
{

    // Size of General purpose registers and the data bus
    using Byte = unsigned char; // or uint8_t

    // Offset anywhere in the address spece or size of an address space
    // using Word = unsigned short; // or uint16_t

    union Word
    {
        static const unsigned int BIT_MASK = 0xFFFF;

        unsigned short word;
        struct
        {
            // TODO verify / test byte ordering....
            unsigned char hi;
            unsigned char lo;
        };

        // ===== Constructors =====
        Word() : word(0) {}

        // --- Copy constructor
        Word(const Word &value)
        {
            word = value;
        }

        Word(const Byte upper, const Byte lower)
        {
            hi = upper;
            lo = lower;
        }

        Word(const int value) : word(value & BIT_MASK) {}

        // ===== Operator overloads =====

        // --- operator = (assignment)
        Word operator=(const Word &value)
        {
            word = value;
            return *this;
        }
        Word operator=(const unsigned int value)
        {
            word = (unsigned short)(value & BIT_MASK);

            return *this;
        }

        // --- operator + (addition)
        Word operator+(const unsigned int value)
        {
            word = (unsigned short)((word + value) & BIT_MASK);

            return *this;
        }
        Word operator+(const unsigned short value)
        {
            word = word + value;

            return *this;
        }
        Word operator+(const Byte value)
        {
            word = (unsigned short)((word + value) & BIT_MASK);

            return *this;
        }
        Word operator+(const Word value)
        {
            word = word + (unsigned short)value;

            return *this;
        }

        // --- operator ++ (increment)
        Word operator++()
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
        Word operator--()
        {
            --word;

            return *this;
        }

        // --- operator += (addition assignment)
        Word &operator+=(const Word rhs)
        {
            word += rhs;

            return *this;
        }
        Word &operator+=(const int rhs)
        {
            word += rhs;
            word &= BIT_MASK;

            return *this;
        }

        // --- operator -= (subtraction assignment)
        Word &operator-=(const int rhs)
        {
            word -= rhs;
            word &= BIT_MASK;

            return *this;
        }

        // --- operator () (type conversion)
        operator unsigned short() const
        {
            return word;
        }
    };

    // Type safe definition of a 16 bit address
    // includes reference to the upper and lower bytes of the address word
    union Address
    {
        Word address;
        struct
        {
            // TODO verify order of these in the Word address
            Byte pch;
            Byte pcl;
        };

        // ===== Constructors =====
        Address() {}
        Address(const Byte upper, const Byte lower)
        {
            pch = upper;
            pcl = lower;
        }
        Address(const int vector)
        {
            address = vector;
        }
        // Address( const m6502::HardwareVector vector )
        // {

        // }

        // --- Copy constructor
        Address(const Address &value)
        {
            address = value.address;
        }

        Address(const unsigned int value)
        {
            address = value & 0xFFFF;
        }
        Address(const Word value)
        {
            address = value & 0xFFFF;
        }

        // ===== Operator overloads =====

        // --- operator = (assignment)
        Address &operator=(const Address &rhs)
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
        Address operator+(const Address value)
        {
            *this += value;

            return *this;
        }
        Address operator+(const Word value)
        {
            address += value;

            return *this;
        }
        Address operator+(const Byte value)
        {
            address = address + value;

            return *this;
        }
        Address operator+(const int value)
        {
            address += value;

            return *this;
        }

        // --- operator - (subtraction)

        // --- operator ++ (increment)
        Address &operator++() // prefix operator
        {
            ++address;

            return *this;
        }

        Address &operator++(int) // prefix operator
        {
            ++address;

            return *this;
        }

        // --- operator -- (decrement)
        Address &operator--() // prefix operator
        {
            --address;

            return *this;
        }
        Address &operator--(int) // prefix operator
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
        Address operator+=(Address &rhs)
        {
            address += rhs.address;

            return *this;
        }
        Address operator+=(const Address &rhs)
        {
            address += rhs.address;

            return *this;
        }
        Address operator+=(const Word &rhs)
        {
            address += rhs;

            return *this;
        }
        Address operator+=(const int rhs)
        {
            address += rhs;

            return *this;
        }

        // --- operator -= (subtraction assignment)
        Address operator-=(const int rhs)
        {
            address -= rhs;

            return *this;
        }

        // --- operator & (bitwise AND)
        Address operator&(const Address &rhs)
        {
            address = address & rhs.address;

            return *this;
        }
        // Address operator&(const Word &rhs)
        // {
        //     address = address & rhs;

        //     return *this;
        // }

        // --- operator int() (conversion)
        operator int() const
        {
            return this->address;
        }
    };

    Address operator+(int val, const Address &addr);

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

}

#endif // HARDWARE_HPP
