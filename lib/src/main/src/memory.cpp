

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
          byteCount(upperBound.address - lowerBound.address + 1)
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
        fill(0);
    }

    void Memory::fill(hardware::Byte value)
    {
        memset(contents, value, byteCount);
    }

    hardware::Byte &Memory::operator[](size_t index)
    {
        if (index >= byteCount)
        {
            throw std::out_of_range("Index out of bounds");
        }

        return contents[index];
    }

    hardware::Byte &Memory::operator[](size_t index) const
    {
        if (index >= byteCount)
        {
            throw std::out_of_range("Index out of bounds");
        }

        return contents[index];
    }

    hardware::Byte &Memory::operator[](hardware::Address &index)
    {
        if ((index.address - lowerBound) >= byteCount)
        {
            throw std::out_of_range("Index out of bounds");
        }

        return contents[index.address - lowerBound];
    }

    hardware::Byte &Memory::operator[](hardware::Address &index) const
    {
        if ((index.address - lowerBound) >= byteCount)
        {
            throw std::out_of_range("Index out of bounds");
        }

        return contents[index.address - lowerBound];
    }

    // read a byte from memory
    hardware::Byte Memory::read(const hardware::Address &address) const
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

    // hardware::Word Memory::readWord(const m6502::hardwareVector address) const
    // {
    // }

    hardware::Word Memory::readWord(const hardware::Address &address) const
    {
        hardware::Word addr(contents[address + 1], contents[address]);

        return addr;
        // hardware::Address xx;
        // xx.pch = contents[address.address];
        // xx.pcl = contents[address.address + 1];

        // TODO setup union for Byte and Word with lo and hi bytes
        // return (hardware::Word)xx.address;

        // hardware::Word value = contents[address] | contents[address + 1] << 8;
        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << "   read( " << std::setfill('0') << std::setw(4) << (int)address << " ) = "
        //           << " LSB:  " << (int)contents[address]
        //           << " MSB:  " << (int)contents[address + 1]
        //           << " Word: " << (int)value << std::endl;
        // std::cout.unsetf(std::ios::basefield);

        // return contents[address] | contents[address + 1] << 8;
    }

    // hardware::Word Memory::readWord(const hardware::Address address) const
    // {
    //     hardware::Word addr(contents[address], contents[address + 1]);
    //     // hardware::Word value = contents[address] | contents[address + 1] << 8;
    //     // std::cout.setf(std::ios::hex, std::ios::basefield);
    //     // std::cout << "   read( " << std::setfill('0') << std::setw(4) << (int)address << " ) = "
    //     //           << " LSB:  " << (int)contents[address]
    //     //           << " MSB:  " << (int)contents[address + 1]
    //     //           << " Word: " << (int)value << std::endl;
    //     // std::cout.unsetf(std::ios::basefield);

    //     // return contents[address] | contents[address + 1] << 8;
    //     return addr;
    // }

    // write a byte to memory, returning the value written
    hardware::Byte Memory::write(const hardware::Address &address, hardware::Byte value)
    {
        return contents[address] = value;
    }

    hardware::Word Memory::writeWord(const hardware::Address &address, hardware::Word value)
    {
        contents[address] = value.lo;
        contents[address + 1] = value.hi;

        return value;
    }

    void Memory::showMemory(const hardware::Address from, const int count, const char *text) const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << bankName << "  contents "
                  << std::setfill('0') << std::setw(4) << from << ".."
                  << std::setfill('0') << std::setw(4) << from + count
                  << " " << text
                  << std::endl;

        hardware::Address addr = from;
        std::cout << std::setfill('0') << std::setw(4) << addr << ": ";
        //        auto end = from + count;
        //        auto diff = addr - from;
        //        for (int i = 0; i <= count; i++)
        int limit = count + 1;
        for (auto diff = addr - from + 1; diff < limit; ++diff)
        {
            std::cout << " " << std::setw(2) << (int)contents[addr];
            //            ++diff;
            ++addr;
            if (diff != 0 && ((diff % 8) == 0))
            //            if (((diff % 8) == 0))
            {
                std::cout << std::endl;
                if (diff < count)
                    std::cout << std::setfill('0') << std::setw(4) << addr << ": ";
            }
        }

        std::cout << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }
}
