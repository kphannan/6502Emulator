
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
        word = value.word;
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

    // --- operator - (subtraction)
    // Word Word::operator-(const Address value)
    Word Word::operator-(const Word value)
    {
        Word tmp(this->word - value.word);

        return tmp;
    }

    Word Word::operator-(const Byte value)
    {
        Word tmp(this->word - value);

        return tmp;
    }

    Word Word::operator-(const int value)
    {
        Word tmp(this->word - value);

        return tmp;
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
    AddressOld::AddressOld()
    {
    }

    AddressOld::AddressOld(const Byte upper, const Byte lower)
    {
        pch = upper;
        pcl = lower;
    }

    AddressOld::AddressOld(const int vector)
    {
        address = vector & BIT_MASK;
    }
    // AddressOld( const m6502::HardwareVector vector )
    // {

    // }

    AddressOld::AddressOld(const hardware::Byte zeroPage)
    {
        pch = 0;
        pcl = zeroPage;
    }

    // --- Copy constructor
    AddressOld::AddressOld(const AddressOld &value)
    {
        address = value.address;
    }

    AddressOld::AddressOld(const unsigned int value)
    {
        address = value & 0xFFFF;
    }

    AddressOld::AddressOld(const Word value)
    {
        address = value; // Simplify / performance
                         //        address = value & 0xFFFF;   // Simplify / performance
    }

    // ===== Operator overloads =====

    // --- operator = (assignment)
    AddressOld &AddressOld::operator=(const AddressOld &rhs)
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
    AddressOld AddressOld::operator+(const AddressOld value)
    {
        AddressOld tmp(this->address.word + value.address.word);

        return tmp;
        // *this += value;

        // return *this;
    }
    AddressOld AddressOld::operator+(const Word value)
    {
        AddressOld tmp(this->address.word + value.word);

        return tmp;
        // address += value;

        // return *this;
    }
    AddressOld AddressOld::operator+(const Byte value)
    {
        AddressOld tmp(this->address.word + value);

        return tmp;
        //         address = address + value;
        //
        //         return *this;
    }
    AddressOld AddressOld::operator+(const int value)
    {
        AddressOld tmp(this->address.word + value);

        return tmp;
        // AddressOld tmp = *this;

        // tmp += value;

        // return tmp;
    }

    // --- operator - (subtraction)
    AddressOld AddressOld::operator-(const AddressOld value)
    {
        AddressOld tmp(this->address - value.address);

        return tmp;
    }

    AddressOld AddressOld::operator-(const Word value)
    {
        AddressOld tmp(this->address - value.word);

        return tmp;
    }

    AddressOld AddressOld::operator-(const Byte value)
    {
        AddressOld tmp(this->address - value);

        return tmp;
    }

    AddressOld AddressOld::operator-(const int value)
    {
        AddressOld tmp(this->address - value);

        return tmp;
    }

    // --- operator ++ (increment)
    AddressOld &AddressOld::operator++() // prefix operator
    {
        ++address;

        return *this;
    }

    AddressOld &AddressOld::operator++(int) // postfix operator
    {
        ++address;

        return *this;
    }

    // --- operator -- (decrement)
    AddressOld &AddressOld::operator--() // prefix operator
    {
        --address;

        return *this;
    }
    AddressOld &AddressOld::operator--(int) // postfix operator
    {
        --address;

        return *this;
    }

    // AddressOld& operator+=( const AddressOld& rhs) // prefix operator
    // {
    //     ++address;

    //     return *this;
    // }

    // --- operator += (addition assignment)
    AddressOld AddressOld::operator+=(AddressOld &rhs)
    {
        address += rhs.address;

        return *this;
    }
    AddressOld AddressOld::operator+=(const AddressOld &rhs)
    {
        address += rhs.address;

        return *this;
    }
    AddressOld AddressOld::operator+=(const Word &rhs)
    {
        address += rhs;

        return *this;
    }
    AddressOld AddressOld::operator+=(const int rhs)
    {
        address += rhs;

        return *this;
    }

    // --- operator -= (subtraction assignment)
    AddressOld AddressOld::operator-=(const int rhs)
    {
        address -= rhs;

        return *this;
    }

    // --- operator & (bitwise AND)
    AddressOld AddressOld::operator&(const AddressOld &rhs)
    {
        address = address & rhs.address;

        return *this;
    }
    // AddressOld operator&(const Word &rhs)
    // {
    //     address = address & rhs;

    //     return *this;
    // }

    // --- operator &= (and assignment)
    AddressOld AddressOld::operator&=(const AddressOld &rhs)
    {
        address.word &= rhs.address.word;

        return *this;
    }

    // --- operator |= (or assignment)
    AddressOld AddressOld::operator|=(const AddressOld &rhs)
    {
        address.word |= rhs.address.word;

        return *this;
    }

    // --- operator int() (conversion)
    AddressOld::operator int() const
    {
        return this->address;
    }

    // =========================================================
    // ===== Address =====
    // =========================================================

    // ----- Constructors -----
    // Address::Address(const unsigned char bits)
    Address::Address()
    {
        size = 16; // bits;
        value.address = 0;
        mask = (1u << size) - 1;
    };

    // Address::Address(const unsigned int value, const unsigned char bits) : Address(bits)
    Address::Address(const unsigned int value) : Address()
    {
        if (mask < value)
        {
            throw std::out_of_range(std::format("Invalid address: {:X} for {} bits", value, size));
        }

        this->value.address = value;
    }

    Address::Address(const hardware::Byte hi, const hardware::Byte lo)
        : Address(hi << 8 | lo)
    {
    }

    Address::Address(const Address &other)
    {
        size = other.size;
        value.address = other.value.address;
    }

    // ----- Methods -----
    Byte Address::lo() const
    {
        return value.address.lo;
    }

    Byte Address::hi() const
    {
        return value.address.hi;
    }

    // ----- Operators -----
    // --- operator [] (subscript)
    // Address &Address::operator[](const unsigned int index)
    // {
    // }
    // Address &Address::operator[](const unsigned int index) const
    // {
    // }
    // --- operator = (assignment)
    Address &Address::operator=(const Address &rhs)
    {
        // TODO check sizes match
        value = rhs.value;

        return *this;
    }

    // --- operator + (addition)
    Address Address::operator+(const Address &rhs) const
    {
        return *this + rhs.value.address.word;
        // Address tmp((unsigned int)(this->value + rhs.value));

        // return tmp;
    }
    Address Address::operator+(const Word rhs) const
    {
        return *this + rhs.word;
        // Address tmp((unsigned int)(this->value + rhs.word));

        // return tmp;
    }
    Address Address::operator+(const Byte rhs) const
    {
        return *this + (unsigned int)rhs;
        // Address tmp((unsigned int)(this->value + rhs));

        // return tmp;
    }
    Address Address::operator+(const int rhs) const
    {
        // return *this + rhs;
        Address tmp((unsigned int)(this->value.address + rhs));

        return tmp;
    }
    Address Address::operator+(const unsigned int rhs) const
    {
        Address tmp(this->value.address.word + rhs);

        return tmp;
    }

    // --- operator ++ (increment) pre-increment
    Address &Address::operator++()
    {
        value.address.word++;

        return *this;
    }

    // --- operator ++ (increment) post-increment
    Address &Address::operator++(int)
    {
        value.address.word++;

        return *this;
    }

    // --- operator += (addition assignment)
    Address &Address::operator+=(const Address &rhs)
    {
        value.address.word += rhs.value.address.word;

        return *this;
    }
    Address &Address::operator+=(const Word rhs)
    {
        value.address.word += rhs;

        return *this;
    }
    Address &Address::operator+=(const Byte rhs)
    {
        value.address.word += rhs;

        return *this;
    }
    Address &Address::operator+=(const int rhs)
    {
        value.address.word += rhs;

        return *this;
    }
    Address &Address::operator+=(const unsigned int rhs)
    {
        value.address.word += rhs;

        return *this;
    }

    // --- operator - (subtraction)
    Address Address::operator-(const Address &rhs) const
    {
        Address tmp((unsigned int)(this->value.address.word - rhs.value.address.word));

        return tmp;
    }
    Address Address::operator-(const Word rhs) const
    {
        Address tmp((unsigned int)(this->value.address - rhs.word));

        return tmp;
    }
    Address Address::operator-(const Byte rhs) const
    {
        Address tmp((unsigned int)(this->value.address - rhs));

        return tmp;
    }
    Address Address::operator-(const int rhs) const
    {
        Address tmp((unsigned int)(this->value.address - rhs));

        return tmp;
    }
    Address Address::operator-(const unsigned int rhs) const
    {
        Address tmp((unsigned int)(this->value.address - rhs));

        return tmp;
    }

    // --- operator -- (decrement - pre)
    Address &Address::operator--()
    {
        value.address.word--;

        return *this;
    }

    // --- operator -- (decrement - post)
    Address &Address::operator--(int)
    {
        value.address.word--;

        return *this;
    }

    // --- operator -= (subtraction assignment)
    Address &Address::operator-=(const Address &rhs)
    {
        value.address.word -= rhs.value.address.word;

        return *this;
    }
    Address &Address::operator-=(const Word rhs)
    {
        value.address.word -= rhs;

        return *this;
    }
    Address &Address::operator-=(const Byte rhs)
    {
        value.address.word -= rhs;

        return *this;
    }
    Address &Address::operator-=(const int rhs)
    {
        value.address.word -= rhs;

        return *this;
    }
    Address &Address::operator-=(const unsigned int rhs)
    {
        value.address.word -= rhs;

        return *this;
    }

    // --- operator == (comparison)
    bool Address::operator==(const Address &rhs) const
    {
        return this->value.address.word == rhs.value.address.word;
    }

    bool Address::operator==(const unsigned int rhs) const
    {
        return this->value.address.word == rhs;
    }
    bool Address::operator==(const int rhs) const
    {
        return this->value.address.word == rhs;
    }

    // ===== StackAddress =====
    // ----- Constructors -----
    // StackAddress::StackAddress() : Address(0x01FF, 16)
    StackAddress::StackAddress() : StackAddress(0x01FF)
    {
    }

    // StackAddress::StackAddress(const unsigned int value) : Address(value, 16)
    StackAddress::StackAddress(const unsigned int value) : Address(value)
    {
        validateAddress(value);
        // if (value > 0x01FF || value < 0x100)
        // {
        //     throw std::out_of_range(std::format("Invalid address: {}", value));
        // }
    }

    // ----- Methods -----
    Byte StackAddress::current()
    {
        return this->value.address.lo;
    }

    bool StackAddress::isValidAddress() const
    {
        return isValidAddress(value.address.word);
    }

    bool StackAddress::isValidAddress(const unsigned int value) const
    {
        return (value <= 0x01FF && value <= 0x100);
    }

    bool StackAddress::validateAddress() const
    {
        return validateAddress(value.address.word);
    }

    bool StackAddress::validateAddress(const unsigned int value) const
    {
        if (!((value >= 0x0100) && (value <= 0x01FF)))
        {
            throw std::out_of_range(std::format("Invalid stack address: {:X}", value));
        }

        return true;
    }

    void StackAddress::maskAddress()
    {
        value.address.word &= mask;
    }

    // ----- Operators -----
    // --- operator = (assignment)
    Address &StackAddress::operator=(const Address &rhs)
    {
        return Address::operator=(rhs);
    }
    Address &StackAddress::operator=(const Byte rhs)
    {
        this->value.address.lo = rhs;

        return *this;
    }

    // --- operator + (addition)
    // Address Address::operator+(const Address &rhs) const;
    // Address Address::operator+(const Word value) const;
    Address StackAddress::operator+(const Byte value) const
    {
        return StackAddress(this->value.address + value);
    }
    // Address Address::operator+(const int value) const;
    Address StackAddress::operator+(const unsigned int value) const
    {
        StackAddress result(this->value.address + value);

        return result;
    }

    // --- operator ++ (increment)
    Address &StackAddress::operator++() // pre-increment
    {
        *this = Address::operator++();

        maskAddress();

        return *this;
    }

    Address &StackAddress::operator++(int) // post-increment
    {
        *this = Address::operator++();

        maskAddress();

        return *this;
    }

    // --- operator ++ (increment)
    // --- operator += (addition assignment)
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

    // --- operator - (subtraction)
    // virtual Address operator-(const Address &rhs) const;
    // virtual Address operator-(const Word value) const;
    // virtual Address operator-(const Byte value) const;
    // virtual Address operator-(const int value) const;
    Address StackAddress::operator-(const unsigned int value) const
    {
        StackAddress result(this->value.address - value);

        result.maskAddress();

        return result;
    }

    // --- operator -- (decrement)
    Address &StackAddress::operator--() // pre-decrement
    {
        *this = Address::operator--();

        maskAddress();

        return *this;
    }
    Address &StackAddress::operator--(int) // post-decrement
    {
        *this = Address::operator--();

        maskAddress();

        return *this;
    }
    // --- operator -- (decrement)

    // --- operator -= (subtraction assignment)
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
        return value.address;
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

hardware::AddressOld hardware::operator+(int val, const hardware::AddressOld &addr)
{
    hardware::AddressOld a = addr.address + val;

    return a;
}
