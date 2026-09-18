
#include <format>

#include "hardware.hpp"


namespace hardware
{

    // ===== Word =====
    // ===== Constructors =====
    // TODO add unit test
    Word::Word() : word(0) {}

    // --- Copy constructor
    // TODO add unit test
    Word::Word(const Word &value)
    {
        word = value.word;
    }

    // TODO add unit test
    Word::Word(const Byte upper, const Byte lower)
    {
        hi = upper;
        lo = lower;
    }

    // TODO add unit test
    Word::Word(const unsigned int value) : word(value & BIT_MASK)
    {
    }
    // TODO add unit test
    Word::Word(const int value) : word(value & BIT_MASK)
    {
    }

    // ===== Operator overloads =====

    /// --- operator = (assignment)
    // TODO add unit test
    Word &Word::operator=(const Word &value)
    {
        word = value.word;
        return *this;
    }

    // TODO add unit test
    Word &Word::operator=(const unsigned int value)
    {
        word = (unsigned short)(value & BIT_MASK);

        return *this;
    }

    /// --- operator + (addition) const

    /// --- operator - (subtraction)


    /// --- operator ++ (increment)
    Word& Word::operator++()        // pre-increment
    {
        ++word;

        return *this;
    }

    Word Word::operator++(int)      // post-increment
    {
        Word pre( *this );

        ++word;

        return pre;
    }

    /// --- operator -- (decrement)
    // TODO add unit test
    Word& Word::operator--()
    {
        --word;

        return *this;
    }

    /// --- operator += (addition assignment)
    // TODO add unit test
    Word &Word::operator+=(const Word rhs)
    {
        word += rhs.word;

        return *this;
    }
    // TODO add unit test
    Word &Word::operator+=(const int rhs)
    {
        word += rhs;
        word &= BIT_MASK;

        return *this;
    }

    /// --- operator -= (subtraction assignment)
    // TODO add unit test
    Word &Word::operator-=(const int rhs)
    {
        word -= rhs;
        word &= BIT_MASK;

        return *this;
    }

    /// --- operator () (type conversion)
    // TODO add unit test
    Word::operator unsigned short() const
    {
        return word;
    }

    /// --- friend operator functions ---






    // =========================================================
    // ===== Address =====
    // =========================================================

    // ----- Constructors -----
    // Address::Address(const unsigned char bits)
    // TODO add unit test
    Address::Address()
    {
        size = 16;
        value.address = 0;
        mask = (1u << size) - 1;
    };

    Address::Address(const unsigned int other) : Address()   // TODO add unit test
    {
        if (mask < other)
        {
            throw std::out_of_range(std::format("Invalid address: {:X} for {} bits", other, size));
        }

        value.address = other;
    }

    // TODO add unit test
    Address::Address(const int other) : Address((unsigned int)other){}

    // TODO add unit test
    Address::Address(const hardware::Byte hi, const hardware::Byte lo)
        : Address(hi << 8 | lo)
    {
    }

    Address::Address(const Word& other) : Address( other.word )
    {}

    // --- Copy constructor ---
    Address::Address(const Address &other)
    {
        size = other.size;
        mask = other.mask;
        value.address = other.value.address;
    }

    // ----- Methods -----
    Byte Address::lo() const   // TODO add unit test
    {
        return value.address.lo;
    }

    Byte Address::hi() const   // TODO add unit test
    {
        return value.address.hi;
    }

    // ----- Operators -----
    /// --- operator [] (subscript)
    // Address &Address::operator[](const unsigned int index)
    // {
    // }
    // Address &Address::operator[](const unsigned int index) const
    // {
    // }
    /// --- operator = (assignment)
    Address &Address::operator=(const Address &rhs)   // TODO add unit test
    {
        // TODO check sizes match
        value = rhs.value;
        size = rhs.size;
        mask = rhs.mask;

        return *this;
    }

    /// --- operator ++ (increment) pre-increment
    Address &Address::operator++()      // TODO add unit test
    {
        value.address.word++;

        return *this;
    }

    /// --- operator ++ (increment) post-increment
    Address Address::operator++(int)    // TODO add unit test
    {
        const Address before(*this);

        value.address.word++;

        return before;
    }

    /// --- operator += (addition assignment)
    Address &Address::operator+=(const Address &rhs)       // TODO add unit test
    {
        value.address.word += rhs.value.address.word;

        return *this;
    }
    Address &Address::operator+=(const Word rhs)           // TODO add unit test
    {
        value.address.word += rhs.word;

        return *this;
    }
    Address &Address::operator+=(const Byte rhs)           // TODO add unit test
    {
        value.address.word += rhs;

        return *this;
    }
    Address &Address::operator+=(const int rhs)            // TODO add unit test
    {
        value.address.word += rhs;

        return *this;
    }
    Address &Address::operator+=(const unsigned int rhs)   // TODO add unit test
    {
        value.address.word += rhs;

        return *this;
    }

    /// --- operator - (subtraction)

    /// --- operator -- (decrement - pre)
    Address &Address::operator--()   // TODO add unit test
    {
        value.address.word--;

        return *this;
    }

    /// --- operator -- (decrement - post)
    Address Address::operator--(int)   // TODO add unit test
    {
        // Post decrement returns the value prior to the decrement
        Address pre( *this );

        value.address.word--;

        return pre;
    }

    /// --- operator -= (subtraction assignment)
    Address &Address::operator-=(const Address &rhs)        // TODO add unit test
    {
        value.address.word -= rhs.value.address.word;

        return *this;
    }
    Address &Address::operator-=(const Word rhs)            // TODO add unit test
    {
        value.address.word -= rhs.word;

        return *this;
    }
    Address &Address::operator-=(const Byte rhs)            // TODO add unit test
    {
        value.address.word -= rhs;

        return *this;
    }
    Address &Address::operator-=(const int rhs)             // TODO add unit test
    {
        value.address.word -= rhs;

        return *this;
    }
    Address &Address::operator-=(const unsigned int rhs)    // TODO add unit test
    {
        value.address.word -= rhs;

        return *this;
    }

    /// --- operator == (comparison)
    bool Address::operator==(const Address &rhs) const      // TODO add unit test
    {
        return this->value.address.word == rhs.value.address.word;
    }

    bool Address::operator==(const unsigned int rhs) const  // TODO add unit test
    {
        return this->value.address.word == rhs;
    }
    bool Address::operator==(const int rhs) const           // TODO add unit test
    {
        return this->value.address.word == rhs;
    }

    // ========================================================================
    // ===== StackAddress =====
    // ========================================================================
    // ----- Constructors -----
    // StackAddress::StackAddress() : Address(0x01FF, 16)
    StackAddress::StackAddress() : StackAddress(0x01FF)
    {
    }

    // StackAddress::StackAddress(const unsigned int value) : Address(value, 16)
    StackAddress::StackAddress(const unsigned int other) : Address(other & 0xFFFF)
    {
        validateAddress(other);
    }

    // ----- Methods -----
    Byte StackAddress::current() const
    {
        return this->value.address.lo;
    }

    bool StackAddress::isValidAddress() const
    {
        return isValidAddress(value.address.word);
    }

    bool StackAddress::isValidAddress(const unsigned int other) const
    {
        return (other <= 0x01FF && other >= 0x100);
    }

    bool StackAddress::validateAddress() const
    {
        return validateAddress(value.address.word);
    }

    bool StackAddress::validateAddress(const unsigned int other) const
    {
        if (!((other >= 0x0100) && (other <= 0x01FF)))
        {
            throw std::out_of_range(std::format("Invalid stack address: {:X}", other));
        }

        return true;
    }

    void StackAddress::maskAddress()
    {
        value.address.word &= mask;
    }

    // ----- Operators -----
    /// --- operator = (assignment)
    Address &StackAddress::operator=(const Address &rhs)
    {
        return Address::operator=(rhs);
    }
    Address &StackAddress::operator=(const Byte rhs)
    {
        this->value.address.lo = rhs;

        return *this;
    }

    /// --- operator + (addition)
    // Address Address::operator+(const Address &rhs) const;
    // Address Address::operator+(const Word value) const;
    // Address StackAddress::operator+(const Byte value) const
    // {
    //     return StackAddress(this->value.address + value);
    // }
    // Address Address::operator+(const int value) const;
    // Address StackAddress::operator+(const unsigned int value) const
    // {
    //     StackAddress result(this->value.address + value);

    //     return result;
    // }

    /// --- operator ++ (increment)
    Address &StackAddress::operator++() // pre-increment
    {
        *this = Address::operator++();

        maskAddress();

        return *this;
    }

    Address StackAddress::operator++(int) // post-increment
    {
        StackAddress pre( *this );
        *this = Address::operator++();

        maskAddress();

        return pre;
    }

    /// --- operator ++ (increment)
    /// --- operator += (addition assignment)
    // virtual Address &operator+=(const Address &rhs);
    // virtual Address &operator+=(const Word value);
    // virtual Address &operator+=(const Byte value);
    // virtual Address &operator+=(const int value);
    Address &StackAddress::operator+=(const unsigned int rhs)
    {
        *this = Address::operator+=(rhs);

        maskAddress();

        return *this;
    }

    /// --- operator - (subtraction)
    // virtual Address operator-(const Address &rhs) const;
    // virtual Address operator-(const Word value) const;
    // virtual Address operator-(const Byte value) const;
    // virtual Address operator-(const int value) const;
    Address StackAddress::operator-(const unsigned int rhs) const
    {
        StackAddress result(this->value.address.word - (unsigned short)rhs);

        result.maskAddress();

        return result;
    }

    /// --- operator -- (decrement)
    Address &StackAddress::operator--() // pre-decrement
    {
        *this = Address::operator--();

        maskAddress();

        return *this;
    }
    Address StackAddress::operator--(int) // post-decrement
    {
        StackAddress pre( *this );

        *this = Address::operator--();

        maskAddress();

        return pre;
    }
    /// --- operator -- (decrement)

    /// --- operator -= (subtraction assignment)
    // virtual Address &operator-=(const Address &rhs);
    // virtual Address &operator-=(const Word value);
    // virtual Address &operator-=(const Byte value);
    // virtual Address &operator-=(const int value);
    Address &StackAddress::operator-=(const unsigned int rhs)
    {
        *this = Address::operator-=(rhs);

        maskAddress();

        return *this;
    }

    Address::operator unsigned int() const
    {
        return value.address.word;
    }



};



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


