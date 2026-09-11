

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
        Word();

        // --- Copy constructor
        Word(const Word &value);
        Word(const Byte upper, const Byte lower);
        Word(const int value);

        // ===== Operator overloads =====

        // --- operator = (assignment)
        Word operator=(const Word &value);
        Word operator=(const unsigned int value);

        // --- operator + (addition)
        Word operator+(const unsigned int value);
        Word operator+(const unsigned short value);
        Word operator+(const Byte value);
        Word operator+(const Word value);

        // --- operator - (subtraction)
        // Word operator-(const Address value);
        Word operator-(const Word value);
        Word operator-(const Byte value);
        Word operator-(const int value);

        // --- operator ++ (increment)
        Word operator++();
        // Word operator++(int)
        // {
        //     ++word;

        //     return *this;
        // }

        // --- operator -- (decrement)
        Word operator--();

        // --- operator += (addition assignment)
        Word &operator+=(const Word rhs);
        Word &operator+=(const int rhs);

        // --- operator -= (subtraction assignment)
        Word &operator-=(const int rhs);

        // --- operator () (type conversion)
        operator unsigned short() const;
    };

    // Type safe definition of a 16 bit address
    // includes reference to the upper and lower bytes of the address word
    union AddressOld
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
        AddressOld();
        AddressOld(const Byte upper, const Byte lower);
        AddressOld(const hardware::Byte zeroPage);
        AddressOld(const int vector);
        AddressOld(const unsigned int value);
        AddressOld(const Word value);

        // --- Copy constructor
        AddressOld(const AddressOld &value);

        // ===== Operator overloads =====

        // --- operator = (assignment)
        AddressOld &operator=(const AddressOld &rhs);
        // Word operator=(const Word value)
        // {
        //     address = value;
        //     return address;
        // }

        // --- operator + (addition)
        AddressOld operator+(const AddressOld value);
        AddressOld operator+(const Word value);
        AddressOld operator+(const Byte value);
        AddressOld operator+(const int value);

        // --- operator - (subtraction)
        AddressOld operator-(const AddressOld value);
        AddressOld operator-(const Word value);
        AddressOld operator-(const Byte value);
        AddressOld operator-(const int value);

        // --- operator ++ (increment)
        AddressOld &operator++(); // prefix operator

        AddressOld &operator++(int); // postfix operator

        // --- operator -- (decrement)
        AddressOld &operator--();    // prefix operator
        AddressOld &operator--(int); // postfix operator

        // AddressOld& operator+=( const AddressOld& rhs) // prefix operator
        // {
        //     ++address;

        //     return *this;
        // }

        // --- operator += (addition assignment)
        AddressOld operator+=(AddressOld &rhs);
        AddressOld operator+=(const AddressOld &rhs);
        AddressOld operator+=(const Word &rhs);
        AddressOld operator+=(const int rhs);

        // --- operator -= (subtraction assignment)
        AddressOld operator-=(const int rhs);

        // --- operator & (bitwise AND)
        AddressOld operator&(const AddressOld &rhs);
        // AddressOld operator&(const Word &rhs)
        // {
        //     address = address & rhs;

        //     return *this;
        // }

        // --- operator &= (and assignment)
        AddressOld operator&=(const AddressOld &rhs);

        // --- operator |= (or assignment)
        AddressOld operator|=(const AddressOld &rhs);

        // --- operator int() (conversion)
        operator int() const;
    };

    AddressOld operator+(int val, const AddressOld &addr);

    // --- operator - (subtraction)
    // Word operator-(const AddressOld &lhs, const AddressOld &rhs)
    // {
    //     (int)(lhs.address) - (int)(rhs.address);
    // }

    // TODO create output stream operator to output the address in hex
    //    std::ostream &operator<<(std::ostream &os, const AddressOld &rhs)
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

    class Address
    {
    public:
        union Addr
        {
            static const unsigned int BIT_MASK = 0xFFFF;

            Word address;
            struct
            {
                // TODO verify order of these in the Word address
                Byte lo;
                Byte hi;
            };

            Addr() : address(0)
            {
            }

            Addr(const unsigned int value) : address(value)
            {
            }

            Addr &operator=(const Addr &rhs)
            {
                if (this != &rhs)
                {
                    this->address = rhs.address;
                }

                return *this;
            }
        };

    protected:
    private:
        // ===== attributes =====
    public:
        Addr value; // TODO Refactor to protect this

    protected:
        unsigned int size;
        unsigned int mask;

    private:
        // ===== Methods =====
    public:
        // Addr(const size_t bits = 16);
        Address();

        // Address(const unsigned int value, const size_t bits = 16);
        Address(const unsigned int value);
        Address(const hardware::Byte hi, const hardware::Byte lo);

        Address(const Address &other);

        Byte lo() const;
        Byte hi() const;

        // ===== Operators =====
    public:
        // --- operator [] (subscript)
        // Address &operator[](const unsigned int index);
        // Address &operator[](const unsigned int index) const;
        // --- operator = (assignment)
        virtual Address &operator=(const Address &rhs);

        // --- operator + (addition)
        virtual Address operator+(const Address &rhs) const;
        virtual Address operator+(const Word value) const;
        virtual Address operator+(const Byte value) const;
        virtual Address operator+(const int value) const;
        virtual Address operator+(const unsigned int value) const;

        // --- operator ++ (increment)
        virtual Address &operator++();    // pre-increment
        virtual Address &operator++(int); // post-increment

        // --- operator += (addition assignment)
        virtual Address &operator+=(const Address &rhs);
        virtual Address &operator+=(const Word value);
        virtual Address &operator+=(const Byte value);
        virtual Address &operator+=(const int value);
        virtual Address &operator+=(const unsigned int rhs);

        // --- operator - (subtraction)
        virtual Address operator-(const Address &rhs) const;
        virtual Address operator-(const Word value) const;
        virtual Address operator-(const Byte value) const;
        virtual Address operator-(const int value) const;
        virtual Address operator-(const unsigned int value) const;

        // --- operator -- (decrement)
        virtual Address &operator--();    // pre-decrement
        virtual Address &operator--(int); // post-decrement

        // --- operator -= (subtraction assignment)
        virtual Address &operator-=(const Address &rhs);
        virtual Address &operator-=(const Word value);
        virtual Address &operator-=(const Byte value);
        virtual Address &operator-=(const int value);
        virtual Address &operator-=(const unsigned int rhs);

        // --- operator == (comparison)
        bool operator==(const Address &rhs) const;
        bool operator==(const unsigned int rhs) const;
        bool operator==(const int rhs) const;

        // --- operator () (type conversion)
        operator unsigned int() const;

        // --- operator ] ()
    protected:
    private:
    };

    class StackAddress : public Address
    {
        // ===== Attributes / Constants =====
    public:
        static const unsigned int stackPage = 0x01;
        static const unsigned int stackAddressMask = 0x01FF;

    protected:
    private:
        // ===== Constructors =====
    public:
        StackAddress();

        StackAddress(const unsigned int value);

        // ===== Methods =====
    public:
        hardware::Byte current();

    protected:
    private:
        bool isValidAddress() const;
        bool isValidAddress(const unsigned int value) const;
        bool validateAddress() const;
        bool validateAddress(const unsigned int value) const;

        void maskAddress();

        // ===== Operators =====
    public:
        // --- operator [] (subscript)
        // Address &operator[](const unsigned int index);
        // Address &operator[](const unsigned int index) const;
        // --- operator = (assignment)
        Address &operator=(const Address &rhs) override;
        Address &operator=(const Byte rhs);

        // --- operator + (addition)
        // virtual Address operator+(const Address &rhs) const;
        // virtual Address operator+(const Word value) const;
        Address operator+(const Byte value) const override;
        // virtual Address operator+(const int value) const;
        Address operator+(const unsigned int value) const override;

        // --- operator ++ (increment)
        Address &operator++() override;    // pre-increment
        Address &operator++(int) override; // post-increment

        // --- operator ++ (increment)
        // --- operator += (addition assignment)
        // virtual Address &operator+=(const Address &rhs);
        // virtual Address &operator+=(const Word value);
        // virtual Address &operator+=(const Byte value);
        // virtual Address &operator+=(const int value);
        Address &operator+=(const unsigned int rhs) override;

        // --- operator - (subtraction)
        // virtual Address operator-(const Address &rhs) const;
        // virtual Address operator-(const Word value) const;
        // virtual Address operator-(const Byte value) const;
        // virtual Address operator-(const int value) const;
        Address operator-(const unsigned int value) const override;

        // --- operator -- (decrement)
        Address &operator--() override;    // pre-decrement
        Address &operator--(int) override; // post-decrement
        // --- operator -- (decrement)

        // --- operator -= (subtraction assignment)
        // virtual Address &operator-=(const Address &rhs);
        // virtual Address &operator-=(const Word value);
        // virtual Address &operator-=(const Byte value);
        // virtual Address &operator-=(const int value);
        Address &operator-=(const unsigned int rhs) override;

        // --- operator == (comparison)
        // bool operator==(const Address &rhs) const;
    };

}

#endif // HARDWARE_HPP
