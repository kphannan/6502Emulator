

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

#include "hardware.hpp"

#define DEFAULT_BANK_NAME "DefaultMemory"

namespace memory
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
        const hardware::Address lowerBound;
        const hardware::Address upperBound;
        const hardware::Word byteCount;

        hardware::Byte *contents;

    protected:
    public:
        // Constructors
    private:
    protected:
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

        ~Memory();

        // Methods
    private:
    protected:
    public:
        // Read a single byte from memory
        hardware::Byte read(const hardware::Address address) const;
        hardware::Word readWord(const hardware::Address memory) const;
        hardware::Byte write(const hardware::Address address, hardware::Byte value);

        void clear();

        // const char *name() const;
        char *name() { return bankName; };

        hardware::Address lowAddress() const;
        hardware::Address highAddress() const;
        hardware::Word memorySize() const;

        void showMemory(const hardware::Address from, const int count) const;

        // Operators
    private:
    protected:
    public:
    };

}

#endif // MEMORY_HPP