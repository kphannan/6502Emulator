

#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <format>
#include <string.h>

#include "memory.hpp"

namespace memory
{
    // ===== Constructors  =====
    Memory::Memory() : Memory(DEFAULT_BANK_NAME)
    {
    }

    Memory::Memory(const hardware::Word size) : Memory(DEFAULT_BANK_NAME, size)
    {
    }

    Memory::Memory(const char *name, hardware::Word size) : Memory(name, hardware::Address(memory_min), size)
    {
    }

    Memory::Memory(const char *name) : Memory(name, hardware::Address(memory_min), hardware::Address(memory_max))
    {
    }

    Memory::Memory(const char *name, const hardware::Address& lowLimit, const hardware::Word& size)
        : Memory(name, lowLimit, lowLimit + size)
    {
    }

    Memory::Memory(const char *name, const hardware::Address& lowerBound, const hardware::Address& upperBound)
        : lowerBound(lowerBound),
          upperBound(upperBound),
          byteCount(upperBound.value.address.word - lowerBound.value.address.word + 1),
          contents( new hardware::Byte[byteCount])
    {
        if (byteCount <= 0)
        {
            throw std::out_of_range(std::format("Invalid size {:#06x} for block [{:#06x},{:#06x}]",
                 byteCount, lowerBound.value.address.word, upperBound.value.address.word));
        }

        // TODO null checks
        strncpy(this->bankName, name, sizeof(bankName) - 1);

        // contents = new hardware::Byte[byteCount];
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
        fill(0);
    }

    void Memory::fill(hardware::Byte value)
    {
        memset(contents, value, byteCount);
    }

    bool Memory::isInBounds(const hardware::Address &address) const
    {
        return lowerBound <= address && address <= upperBound;
    }

    bool Memory::isInBounds(const size_t address) const
    {
        return isInBounds( hardware::Address((unsigned int)address) );
        // return lowerBound <= address && address <= upperBound;
    }

    hardware::Byte &Memory::operator[](const size_t address)
    {
        // TODO Chain to address argument
        // throw std::domain_error(std::format("Testing...1...{:#06x}", address));
        // if (!isInBounds(static_cast<unsigned int>(address))) // TODO type conversion
        if (!isInBounds(address)) // TODO type conversion
        {
            throw std::out_of_range(std::format("address {:#06x} not in block [{:#06x},{:#06x}]", address, lowerBound.value.address.word, upperBound.value.address.word));
        }

        return contents[address - lowerBound];
    }

    // const hardware::Byte &Memory::operator[](const size_t address) const
    // {
    //     // TODO Chain to address argument
    //     // throw std::domain_error(std::format("Testing...2...{:#06x}", address));
    //     if (!isInBounds(address))
    //     {
    //         throw std::out_of_range(std::format("address {:#06x} not in block [{:#06x},{:#06x}]", address, lowerBound.value.address.word, upperBound.value.address.word));
    //     }

    //     return contents[address - lowerBound];
    // }

    // hardware::Byte &Memory::operator[](const hardware::Address& address)
    // {
    //     // throw std::domain_error(std::format("Testing...1...{:#06x}", address));
    //     // if (!isInBounds(static_cast<unsigned int>(address))) // TODO type conversion
    //     if (!isInBounds(address)) // TODO type conversion
    //     {
    //         throw std::out_of_range(
    //             std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
    //                  address.value.address.word,
    //                  lowerBound.value.address.word,
    //                  upperBound.value.address.word));
    //     }

    //     return contents[address - lowerBound];
    // }

    // const hardware::Byte &Memory::operator[](const hardware::Address& address) const
    // {
    //     // throw std::domain_error(std::format("Testing...2...{:#06x}", address));
    //     if (!isInBounds(address))
    //     {
    //         throw std::out_of_range(std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
    //              address.value.address.word,
    //              lowerBound.value.address.word,
    //              upperBound.value.address.word));
    //     }

    //     return contents[address - lowerBound];
    // }

    // Memory& Memory::operator=(const unsigned int rhs )



    // read a byte from memory
    hardware::Byte Memory::read(const hardware::Address &address) const
    {
        if (isInBounds(address))
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

        // https://stackoverflow.com/questions/11938979/what-exception-classes-are-in-the-standard-c-library
        // https://en.cppreference.com/w/cpp/error/exception

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << "read " << std::setfill('0') << std::setw(4) << lowerBound << " <= " << address << " <= " << upperBound << std::endl;
        // std::cout.unsetf(std::ios::basefield);

        // TODO fix all the dereferencing
        // std::out_of_range or std::length_error
        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));
    }


    hardware::Word Memory::readWord(const hardware::Address &address) const
    {
        hardware::Word addr(contents[address + 1], contents[address]);

        return addr;
    }
    hardware::Address Memory::readAddress(const hardware::Address &address) const
    {
        hardware::Address addr(contents[address + 1], contents[address]);

        return addr;
    }


    // write a byte to memory, returning the value written
    hardware::Byte Memory::write(const hardware::Address &address, const hardware::Byte value)
    {
        return contents[address] = value;
    }

    hardware::Word Memory::writeWord(const hardware::Address &address, const hardware::Word value)
    {
        contents[address] = value.lo;
        contents[address + 1] = value.hi;

        return value;
    }

    hardware::Address Memory::writeAddress(const hardware::Address& address, const hardware::Address& value)
    {
        contents[address] = value.value.address.lo;
        contents[address + 1] = value.value.address.hi;

        return value;
    }

    void Memory::showMemory(const hardware::Address& from, const int count, const char *text) const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << bankName << "  contents "
                  << std::setfill('0') << std::setw(4) << from << ".."
                  << std::setfill('0') << std::setw(4) << from + (count - 1)
                  << " " << text
                  << std::endl;

        hardware::Address addr = from;
        std::cout << std::setfill('0') << std::setw(4) << addr << ": ";
        int limit = count + 1;
        for (int diff = addr - from + 1; diff < limit; ++diff)
        {
            std::cout << " " << std::setw(2) << (int)contents[addr];
            ++addr;
            if (diff != 0 && ((diff % 8) == 0))
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
