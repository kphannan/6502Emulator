

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#include "hardware.hpp"

#define DEFAULT_BANK_NAME "DefaultMemory"

namespace cpu
{
    class Memory
    {
        // Constants
    private:
    public:
    private:
        // static const char *DEFAULT_BANK_NAME = "DefaultMemory";

        // Fields
    private:
    public:
    private:
        char bankName[32 + 1];
        const Address lowerBound;
        const Address upperBound;
        const Word byteCount;

        Byte *contents;

    protected:
    public:
        // Constructors
    private:
    protected:
    public:
        // --- Constructors
        // default memory that encompases the entire address space
        Memory();
        Memory(const Word byteCount);
        Memory(const char *name, const Word byteCount);
        // named memory that spans the entire address space
        Memory(const char *name);
        // named subset of the address space
        Memory(const char *name, const Address lowLimit, const Address upperLimit);

        ~Memory();

        // Methods
    private:
    protected:
    public:
        // Read a single byte from memory
        Byte read(const Address address) const;
        Word readWord(const Address memory) const;
        Byte write(const Address address, Byte value);

        void clear();

        // const char *name() const;
        char *name();

        Address lowAddress() const;
        Address highAddress() const;
        Word memorySize() const;

        // Operators
    private:
    protected:
    public:
    };

}

#endif // MEMORY_HPP