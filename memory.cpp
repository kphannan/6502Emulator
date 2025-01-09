

#include <stdlib.h>

#include "memory.hpp"
#include <string.h>

namespace cpu
{
    // ===== Constructors  =====
    Memory::Memory() : Memory(DEFAULT_BANK_NAME)
    {
    }

    Memory::Memory(const Word byteCount) : Memory(DEFAULT_BANK_NAME, byteCount)
    {
    }

    Memory::Memory(const char *name, Word byteCount) : Memory(name, 0, byteCount)
    {
    }

    // TODO create constants for min (0x0000) and max (0xFFFF) address
    Memory::Memory(const char *name) : Memory(name, 0x0000, 0xFFFF)
    {
    }

    Memory::Memory(const char *name, const Address lowerBound, const Address upperBound)
        : lowerBound(lowerBound),
          upperBound(upperBound),
          byteCount(upperBound - lowerBound)
    {
        // TODO null checks
        strncpy(this->bankName, name, sizeof(bankName) - 1);

        contents = new Byte(byteCount);
    }

    // --- destructor
    Memory::~Memory()
    {
        // delete contents;
    }

    // ===== Methods =====

    // Blank out memory
    void Memory::clear()
    {
        memset(contents, 0, byteCount);
    }

    // get the name of the mory bank
    // const char *name() const
    char *Memory::name()
    {
        return this->bankName;
    }

    // read a byte from memory
    Byte Memory::read(const Word address) const
    {
        return contents[address];
    }

    Word Memory::readWord(const Word address) const
    {
        return contents[address] << 8 | contents[address + 1];
    }

    // write a byte to memory, returning the value written
    Byte Memory::write(const Word address, Byte value)
    {
        // memory[ address ] = value;

        return contents[address] = value;
    }

    // Get the starting address of the memory bank
    Word Memory::lowAddress() const
    {
        return lowerBound;
    }

    // Get the ending address of the memory bank
    Word Memory::highAddress() const
    {
        return upperBound;
    }

    // Get the number of bytes in the memory bank
    Word Memory::memorySize() const
    {
        return byteCount;
    }

}
