

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#include "hardware.hpp"

#define DEFAULT_BANK_NAME "DefaultMemory"

namespace memory
{
    // TODO possibly convert to template with Memory<Address A, Byte D>.....
    class Memory
    {
        // Constants
    public:
    protected:
    private:
        // static const char *DEFAULT_BANK_NAME = "DefaultMemory";

        // Fields
    public:
    protected:
    private:
        char bankName[32 + 1]; // TODO could be std::string
        const hardware::Address lowerBound;
        const hardware::Address upperBound;
        const int_fast32_t byteCount;

        hardware::Byte *contents;

        // Constructors
    public:
        // --- Constructors
        // default memory that encompases the entire address space
        Memory();
        Memory(const hardware::Word byteCount);
        Memory(const char *name, const hardware::Word byteCount);
        // named memory that spans the entire address space
        Memory(const char *name);
        // named subset of the address space
        Memory(const char *name, const hardware::Address lowLimit, const hardware::Address upperLimit);

        // ~Memory();

    protected:
    private:
        // Methods
    public:
        // hardware::Byte read(const hardware::Word address) const;
        // Read a single byte from memory
        hardware::Byte read(const hardware::Address address) const;
        hardware::Word readWord(const hardware::Address address) const;
        hardware::Byte write(const hardware::Address address, hardware::Byte value);
        hardware::Word writeWord(const hardware::Address address, hardware::Word value);

        void clear();
        void fill(hardware::Byte value);

        // const char *name() const;
        char *name() { return bankName; };

        // Get the starting address of the memory bank
        hardware::Address lowAddress() const { return lowerBound; }
        // Get the ending address of the memory bank
        hardware::Address highAddress() const { return upperBound; }
        // Get the number of bytes in the memory bank
        int_fast32_t memorySize() const { return byteCount; }

        void showMemory(const hardware::Address from, const int count, const char *text = "") const;

        // ===== Operator Overloads
        // --- operator [] (array)
        hardware::Byte &operator[](size_t index);
        hardware::Byte &operator[](size_t index) const;

        // --- operator = (assignment)
        // Memory& operator=(const int rhs )
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
