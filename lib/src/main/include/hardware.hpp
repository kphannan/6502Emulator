

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
            #if defined(__BYTE_ORDER__)
                #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                    // Little Endian
                    unsigned char lo;
                    unsigned char hi;
                #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                    // Big Endian
                    unsigned char hi;
                    unsigned char lo;
                #endif
            #endif



        };

        // ===== Constructors =====
        Word();
        Word(const Byte upper, const Byte lower);
        // explicit Word(unsigned short value);
        Word(const unsigned int value);   // TODO explicit
        Word(const int value);    // TODO explicit

        // explicit Word(const unsigned int value);
        // explicit Word(const unsigned short value);


        // --- Copy constructor
        Word(const Word& value);

        // ===== Operator overloads =====

        /// --- operator = (assignment)
        Word& operator=(const Word &value);
        Word& operator=(const unsigned int value);

        /// --- operator ++ (increment)
        Word& operator++();     // pre
        Word operator++(int);   // post

        /// --- operator -- (decrement)
        Word& operator--();     // pre
        Word operator--(int);   // post

        /// --- operator += (addition assignment)
        Word &operator+=(const Word rhs);
        Word &operator+=(const int rhs);

        /// --- operator -= (subtraction assignment)
        Word &operator-=(const int rhs);

        /// --- operator () (type conversion)
        explicit operator unsigned short() const;

        // ===== Hidden friend functions =====

        /// --- operator==() (equality)
        friend bool operator==( const hardware::Word& lhs, int rhs)
        {
            return lhs.word == rhs;
        }
        friend bool operator==(int lhs, const hardware::Word& rhs)
        {
            return operator==( rhs, lhs );
        }

        /// --- operator + (addition)
        friend Word operator+(const Word& lhs, const unsigned int rhs)
        {
            Word result(lhs.word + rhs & 0xFFFF);

            return result;
        }

        friend Word operator+(const Word& lhs, const unsigned short rhs)
        {
            return operator+( lhs, (const unsigned int)rhs);
        }

        friend Word operator+(const Word& lhs, const Byte rhs)
        {
            return operator+( lhs, (const unsigned int)rhs);
        }

        friend Word operator+(const Word& lhs, const Word rhs)
        {
            return operator+( lhs, rhs.word);
        }

        /// --- operator - (subtraction)
        friend Word operator-(const Word& lhs, const Word rhs)
        {
            return operator-( lhs, rhs.word);
        }

        // friend Word operator-(const Word& lhs, const Byte rhs)
        // {
        //     return operator-( lhs, (const unsigned int)rhs);
        // }

        friend Word operator-(const Word& lhs, const unsigned int rhs)
        {
            Word result(lhs.word - rhs & 0xFFFF);

            return result;
        }
    };


    // Type safe definition of a 16 bit address
    // includes reference to the upper and lower bytes of the address word

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
                    // Little-endian lo, hi
                    Byte lo;
                    Byte hi;
                };

                Addr() : address(static_cast<unsigned short>(0))
                {
                }

                explicit Addr(const unsigned int value) : address(value & BIT_MASK)
                {
                }

                // explicit Addr(const Word value ) : A

                Addr &operator=(const Addr &rhs)
                {
                    if (this != &rhs)
                    {
                        this->address = rhs.address;
                    }

                    return *this;
                }
            };

            // ===== attributes =====
            Addr value; // TODO Refactor to protect this

        protected:
            unsigned int size;
            unsigned int mask;

            // ===== Methods =====
        public:
            // Addr(const size_t bits = 16);
            Address();

            // Address(const unsigned int value, const size_t bits = 16);
            explicit Address(int value);
            Address(const unsigned int value);  // TODO explicit
            explicit Address(const Word& value);
            Address(const hardware::Byte hi, const hardware::Byte lo);

            Address(const Address &other);

            Byte lo() const;
            Byte hi() const;

            // ===== Operators =====
            /// --- operator [] (subscript)
            // Address &operator[](const unsigned int index);
            // Address &operator[](const unsigned int index) const;
            /// --- operator = (assignment)
            virtual Address &operator=(const Address &rhs);

            /// --- operator + (addition)    friends

            /// --- operator ++ (increment)
            virtual Address &operator++();   // pre-increment
            virtual Address operator++(int); // post-increment

            /// --- operator += (addition assignment)
            virtual Address &operator+=(const Address &rhs);
            virtual Address &operator+=(const Word value);
            virtual Address &operator+=(const Byte value);
            virtual Address &operator+=(const int value);
            virtual Address &operator+=(const unsigned int rhs);

            /// --- operator - (subtraction)    friends

            /// --- operator -- (decrement)
            virtual Address &operator--();    // pre-decrement
            virtual Address operator--(int); // post-decrement

            /// --- operator -= (subtraction assignment)
            virtual Address &operator-=(const Address &rhs);
            virtual Address &operator-=(const Word value);
            virtual Address &operator-=(const Byte value);
            virtual Address &operator-=(const int value);
            virtual Address &operator-=(const unsigned int rhs);

            /// --- operator == (comparison)
            bool operator==(const Address &rhs) const;
            bool operator==(const unsigned int rhs) const;
            bool operator==(const int rhs) const;

            /// --- operator () (type conversion)
            operator unsigned int() const;

            /// --- operator [] ()

        // ===== friend operator =====
        public:
            /// --- operator + (addition)
            // virtual Address operator+(const Address &rhs) const;

            friend Address operator+(const Address& lhs, const unsigned int rhs)
            {
                Address result(lhs.value.address.word + rhs & 0xFFFF);

                return result;
            }
            friend Address operator+(const Address& lhs, const int rhs)
            {
                Address result(lhs.value.address.word + rhs & 0xFFFF);

                return result;
            }

            friend Address operator+(const Address& lhs, const Byte rhs)
            {
                return operator+( lhs, (const unsigned int)rhs);
            }

            friend Address operator+(const Address& lhs, const Word rhs)
            {
                return operator+( lhs, (const unsigned int)(rhs.word));
            }


            friend std::ostream &operator<<(std::ostream &os, const Address &rhs)
            {
                // std::cout << "        PC: " << std::setfill('0') << std::setw(4) << (int)(registers.PC) << " Program Counter" << std::endl;
                // std::cout.setf(std::ios::hex, std::ios::basefield);
                // os << "0x" << std::setfill('0') << std::setw(4) << rhs.value.address.word;
                // std::cout.unsetf(std::ios::basefield);

                // os << std::format("{:#06x}", rhs.value.address.word, 4 + 2 );
                os << std::format("{:#0{}x}", rhs.value.address.word, 4 + 2 );

                return os;
            }

            /// --- operator - (subtraction)
            // virtual Address operator-(const Address &rhs) const;
            friend Address operator-(const Address& lhs, const unsigned int rhs)
            {
                Address result(lhs.value.address.word - rhs & 0xFFFF);

                return result;
            }

            friend Address operator-(const Address& lhs, const int rhs)
            {
                Address result(lhs.value.address.word - rhs & 0xFFFF);

                return result;
            }

            friend Address operator-(const Address& lhs, const Byte rhs)
            {
                return operator-( lhs, (const unsigned int)rhs);
            }

            friend Address operator-(const Address& lhs, const Word rhs)
            {
                return operator-( lhs, (const unsigned int)(rhs.word));
            }


    };

    class StackAddress : public Address
    {
        // ===== Attributes / Constants =====
        public:
            static const unsigned int stackPage = 0x01;
            static const unsigned int stackAddressMask = 0x01FF;

            // ===== Constructors =====
        public:
            StackAddress();

            explicit StackAddress(const unsigned int value);

            // ===== Methods =====
            hardware::Byte current() const;

        private:
            bool isValidAddress() const;
            bool isValidAddress(const unsigned int value) const;
            bool validateAddress() const;
            bool validateAddress(const unsigned int value) const;

            void maskAddress();

            // ===== Operators =====
        public:
            /// --- operator [] (subscript)
            // Address &operator[](const unsigned int index);
            // Address &operator[](const unsigned int index) const;
            /// --- operator = (assignment)
            Address &operator=(const Address &rhs) override;
            Address &operator=(const Byte rhs);

            /// --- operator + (addition)
            // virtual Address operator+(const Address &rhs) const;
            // virtual Address operator+(const Word value) const;
            // Address operator+(const Byte value) const override;
            // friend Address operator+(const StackAddress& address, const Byte value);
            // friend Address operator+(const StackAddress& address, const unsigned int value);
            // virtual Address operator+(const int value) const;
            // Address operator+(const unsigned int value) const override;

            /// --- operator ++ (increment)
            Address &operator++() override;   // pre-increment
            Address operator++(int) override; // post-increment

            /// --- operator ++ (increment)
            /// --- operator += (addition assignment)
            // virtual Address &operator+=(const Address &rhs);
            // virtual Address &operator+=(const Word value);
            // virtual Address &operator+=(const Byte value);
            // virtual Address &operator+=(const int value);
            Address &operator+=(const unsigned int rhs) override;

            /// --- operator - (subtraction)
            // virtual Address operator-(const Address &rhs) const;
            // virtual Address operator-(const Word value) const;
            // virtual Address operator-(const Byte value) const;
            // virtual Address operator-(const int value) const;
            Address operator-(const unsigned int value) const;

            /// --- operator -- (decrement)
            Address &operator--() override;    // pre-decrement
            Address operator--(int) override; // post-decrement
            /// --- operator -- (decrement)

            /// --- operator -= (subtraction assignment)
            // virtual Address &operator-=(const Address &rhs);
            // virtual Address &operator-=(const Word value);
            // virtual Address &operator-=(const Byte value);
            // virtual Address &operator-=(const int value);
            Address &operator-=(const unsigned int rhs) override;

            /// --- operator == (comparison)
            // bool operator==(const Address &rhs) const;



            /// --- operator + (addition)
            friend StackAddress operator+(const StackAddress &lhs, const int rhs)
            {
                StackAddress tmp(lhs.value.address.word + rhs);

                return tmp;
            }
            friend StackAddress operator+(const StackAddress &lhs, const unsigned int rhs)
            {
                StackAddress tmp(lhs.value.address.word + rhs);

                return tmp;
            }
            friend StackAddress operator+(const StackAddress &lhs, const Word rhs)
            {
                return operator+(lhs, (unsigned int)rhs.word);
            }
            friend StackAddress operator+(const StackAddress &lhs, const Address &rhs)
            {
                return operator+(lhs, (unsigned int)(rhs.value.address.word & 0xFFFF));
            }
            friend StackAddress operator+(const StackAddress &lhs, const Byte rhs)
            {
                return operator+(lhs, (unsigned int)rhs);
            }


    };


}

#endif // HARDWARE_HPP
