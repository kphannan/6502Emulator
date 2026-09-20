

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#include "hardware.hpp"


namespace memory
{
    // TODO possibly convert to template with Memory<Address A, Byte D>.....
    class Memory
    {
        // Constants
        public:
            static constexpr unsigned int memory_min = 0x0000;
            static constexpr unsigned int memory_max = 0xFFFF;

            static constexpr char DEFAULT_BANK_NAME[] = "DefaultMemory";

        protected:
        private:

        // Fields
        public:
        protected:
        private:
            char bankName[32 + 1]; // TODO could be std::string
            const hardware::Address lowerBound;
            const hardware::Address upperBound;
            const int_fast32_t byteCount;

            hardware::Byte* contents;

            // Constructors
        public:
            // --- Constructors
            // default memory that encompases the entire address space
            Memory();
            explicit Memory(const hardware::Word byteCount);
            Memory(const char *name, const hardware::Word byteCount);
            // named memory that spans the entire address space
            explicit Memory(const char *name);
            // named subset of the address space
            Memory(const char *name, const hardware::Address& lowLimit, const hardware::Address& upperLimit);
            Memory(const char *name, const hardware::Address& lowLimit, const hardware::Word& byteCount);

            // Copy constructor
            Memory( const Memory& other );

            // Move constructor
            Memory( Memory&& other );
            // Memory& operator=(const Memory& rhs );   // Copy assignment
            // Memory& operator=(Memory&& rhs );        // Move assignment

            // Destructor
            ~Memory();

            // add assignment operator

        protected:
        private:
            // Methods
        public:
            bool isInBounds(const hardware::Address &address) const;
            bool isInBounds(const size_t address) const;
            // hardware::Byte read(const hardware::Word address) const;
            // Read a single byte from memory
            hardware::Byte read(const hardware::Address& address) const;
            hardware::Word readWord(const hardware::Address& address) const;
            hardware::Address readAddress(const hardware::Address& address) const;
            hardware::Byte write(const hardware::Address& address, const hardware::Byte value);
            hardware::Word writeWord(const hardware::Address& address, const hardware::Word value);
            hardware::Address writeAddress(const hardware::Address& address, const hardware::Address& value);

            void clear();
            void fill(hardware::Byte value);

            const char *name() const { return bankName; };
            // char *name() { return bankName; };

            // Get the starting address of the memory bank
            hardware::Address lowAddress() const { return lowerBound; }
            // Get the ending address of the memory bank
            hardware::Address highAddress() const { return upperBound; }
            // Get the number of bytes in the memory bank
            int_fast32_t memorySize() const { return byteCount; }

            void showMemory(const hardware::Address& from, const int count, const unsigned char bytesPerLine, const char *text) const;
            void showMemory(const hardware::Address& from, const int count, const unsigned char bytesPerLine) const;
            void showMemory(const hardware::Address& from, const int count, const char *text = "") const;

            // ===== Operator Overloads

            /// --- operator [] (subscript)
            hardware::Byte &operator[](const size_t index);
            // const hardware::Byte &operator[](const size_t index) const;
            // hardware::Byte &operator[](const hardware::Address& address);
            // const hardware::Byte &operator[](const hardware::Address& address) const;

            /// --- operator = (assignment)
            // Memory& operator=(const unsigned int rhs );
            // Memory& operator=(const Byte& rhs );
            // {
            //     // TODO range check

            // }

        protected:
        private:
            // Operators
        public:
        protected:
        private:
    };

}

#endif // MEMORY_HPP
