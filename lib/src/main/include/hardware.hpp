

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
            // For Little-Endian 'lo' must be declared
            // before 'hi'
            // (at least on apple silicon)
            unsigned char lo;
            unsigned char hi;
        };

        // ===== Constructors =====
        Word(); // : word(0);
        //{}

        // --- Copy constructor
        Word(const Word &value);
        // {
        //     word = value;
        // }

        Word(const Byte upper, const Byte lower);
        // {
        //     hi = upper;
        //     lo = lower;
        // }

        Word(const int value); // : word(value & BIT_MASK);
        // {}

        // ===== Operator overloads =====

        // --- operator = (assignment)
        Word operator=(const Word &value);
        // {
        //     word = value;
        //     return *this;
        // }
        Word operator=(const unsigned int value);
        // {
        //     word = (unsigned short)(value & BIT_MASK);

        //     return *this;
        // }

        // --- operator + (addition)
        Word operator+(const unsigned int value);
        // {
        //     word = (unsigned short)((word + value) & BIT_MASK);

        //     return *this;
        // }
        Word operator+(const unsigned short value);
        // {
        //     word = word + value;

        //     return *this;
        // }
        Word operator+(const Byte value);
        // {
        //     word = (unsigned short)((word + value) & BIT_MASK);

        //     return *this;
        // }
        Word operator+(const Word value);
        // {
        //     word = word + (unsigned short)value;

        //     return *this;
        // }

        // --- operator ++ (increment)
        Word operator++();
        // {
        //     ++word;

        //     return *this;
        // }
        // Word operator++(int)
        // {
        //     ++word;

        //     return *this;
        // }

        // --- operator -- (decrement)
        Word operator--();
        // {
        //     --word;

        //     return *this;
        // }

        // --- operator += (addition assignment)
        Word &operator+=(const Word rhs);
        // {
        //     word += rhs;

        //     return *this;
        // }
        Word &operator+=(const int rhs);
        // {
        //     word += rhs;
        //     word &= BIT_MASK;

        //     return *this;
        // }

        // --- operator -= (subtraction assignment)
        Word &operator-=(const int rhs);
        // {
        //     word -= rhs;
        //     word &= BIT_MASK;

        //     return *this;
        // }

        // --- operator () (type conversion)
        operator unsigned short() const;
        // {
        //     return word;
        // }
    };

    // Type safe definition of a 16 bit address
    // includes reference to the upper and lower bytes of the address word
    union Address
    {
        static const unsigned int BIT_MASK = 0xFFFF;

        Word address;
        struct
        {
            // TODO verify order of these in the Word address
            Byte pcl;
            Byte pch;
        };

        // ===== Constructors =====
        Address();
        Address(const Byte upper, const Byte lower);
        Address(const hardware::Byte zeroPage);
        Address(const int vector);
        Address(const unsigned int value);
        Address(const Word value);

        // --- Copy constructor
        Address(const Address &value);

        // ===== Operator overloads =====

        // --- operator = (assignment)
        Address &operator=(const Address &rhs);
        // Word operator=(const Word value)
        // {
        //     address = value;
        //     return address;
        // }

        // --- operator + (addition)
        Address operator+(const Address value);
        Address operator+(const Word value);
        Address operator+(const Byte value);
        Address operator+(const int value);

        // --- operator - (subtraction)

        // --- operator ++ (increment)
        Address &operator++(); // prefix operator

        Address &operator++(int); // postfix operator

        // --- operator -- (decrement)
        Address &operator--();    // prefix operator
        Address &operator--(int); // postfix operator

        // Address& operator+=( const Address& rhs) // prefix operator
        // {
        //     ++address;

        //     return *this;
        // }

        // --- operator += (addition assignment)
        Address operator+=(Address &rhs);
        Address operator+=(const Address &rhs);
        Address operator+=(const Word &rhs);
        Address operator+=(const int rhs);

        // --- operator -= (subtraction assignment)
        Address operator-=(const int rhs);

        // --- operator & (bitwise AND)
        Address operator&(const Address &rhs);
        // Address operator&(const Word &rhs)
        // {
        //     address = address & rhs;

        //     return *this;
        // }

        // --- operator &= (and assignment)
        Address operator&=(const Address &rhs);

        // --- operator |= (or assignment)
        Address operator|=(const Address &rhs);

        // --- operator int() (conversion)
        operator int() const;
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
