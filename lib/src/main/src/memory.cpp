

#include <stdlib.h>

#include <iostream>
#include <iomanip>

#include "memory.hpp"
#include <string.h>

namespace memory
{
    // ===== Constructors  =====
    Memory::Memory() : Memory(DEFAULT_BANK_NAME)
    {
    }

    Memory::Memory(const hardware::Word byteCount) : Memory(DEFAULT_BANK_NAME, byteCount)
    {
    }

    Memory::Memory(const char *name, hardware::Word byteCount) : Memory(name, 0, byteCount)
    {
    }

    // TODO create constants for min (0x0000) and max (0xFFFF) address
    Memory::Memory(const char *name) : Memory(name, 0x0000, 0xFFFF)
    {
    }

    Memory::Memory(const char *name, const hardware::Address lowerBound, const hardware::Address upperBound)
        : lowerBound(lowerBound),
          upperBound(upperBound),
          byteCount(upperBound - lowerBound)
    {
        // TODO null checks
        strncpy(this->bankName, name, sizeof(bankName) - 1);

        contents = new hardware::Byte[byteCount];
        // std::cout << "contents: " << contents << " size: " << byteCount << std::endl;
        clear();
    }

    // --- destructor
    // Memory::~Memory()
    // {
    //     // delete contents;
    // }

    // ===== Methods =====

    // Blank out memory
    void Memory::clear()
    {
        // std::cout << "clear contents: " << contents << " size: " << byteCount << std::endl;
        memset(contents, 0, byteCount);
    }

    // read a byte from memory
    hardware::Byte Memory::read(const hardware::Word address) const
    {
        // addres - lowerBound;
        // hardware::Word
        if ((lowerBound <= address && address <= upperBound))
        {
            // hardware::Byte value = contents[address];
            // std::cout.setf(std::ios::hex, std::ios::basefield);
            // std::cout << "   read( " << std::setfill('0') << std::setw(4) << (int)address << " ) = "
            //           // << " byte:  " << (int)contents[address]
            //           // << " MSB:  " << (int)contents[address + 1]
            //           << " Byte: " << (int)value << std::endl;
            // std::cout.unsetf(std::ios::basefield);

            return contents[address - lowerBound];
        }
        else
        {
            // https://stackoverflow.com/questions/11938979/what-exception-classes-are-in-the-standard-c-library
            // https://en.cppreference.com/w/cpp/error/exception
            //
            // TODO possibly throw an out of bounds exception
            // std::out_of_range or std::length_error
            // std::cout << "read " << address << " from contents: " << contents << " size: " << byteCount << std::endl;

            // std::cout.setf(std::ios::hex, std::ios::basefield);
            // std::cout << "read " << std::setfill('0') << std::setw(4) << lowerBound << " <= " << address << " <= " << upperBound << std::endl;
            // std::cout.unsetf(std::ios::basefield);

            throw std::out_of_range("address not in block");
            // return -1;
        }
    }

    hardware::Word Memory::readWord(const hardware::Word address) const
    {
        hardware::Word value = contents[address] | contents[address + 1] << 8;
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "   read( " << std::setfill('0') << std::setw(4) << (int)address << " ) = "
                  << " LSB:  " << (int)contents[address]
                  << " MSB:  " << (int)contents[address + 1]
                  << " Word: " << (int)value << std::endl;
        std::cout.unsetf(std::ios::basefield);

        return contents[address] | contents[address + 1] << 8;
    }

    // write a byte to memory, returning the value written
    hardware::Byte Memory::write(const hardware::Word address, hardware::Byte value)
    {
        return contents[address] = value;
    }

    void Memory::showMemory(const hardware::Address from, const int count) const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << bankName << "  contents "
                  << std::setfill('0') << std::setw(4) << from << ".."
                  << std::setfill('0') << std::setw(4) << from + count
                  << std::endl;

        hardware::Address addr = from;
        std::cout << std::setfill('0') << std::setw(4) << addr << ": ";
        for (int i = 1; i <= count; i++)
        {
            std::cout << " " << std::setw(2) << (int)contents[addr++];
            if (((i % 8) == 0) && ((addr - 1) != from))
            {
                std::cout << std::endl;
                if (i > 0)
                    std::cout << std::setfill('0') << std::setw(4) << addr << ": ";
            }
        }

        std::cout << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

}
