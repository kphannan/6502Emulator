

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
          byteCount(std::abs(upperBound.value.address.word - lowerBound.value.address.word + 1))
    {
        if (lowerBound > upperBound )
        {
            throw std::out_of_range(std::format("Invalid size {:#06x} for block [{:#06x},{:#06x}]",
                 byteCount, lowerBound.value.address.word, upperBound.value.address.word));
        }

        const char* tmpName = nullptr == name ? DEFAULT_BANK_NAME : name;

        // TODO null checks
        strncpy(bankName, tmpName, sizeof(bankName) - 1);
        bankName[sizeof(bankName) - 1] = '\0';  // ensure null terminateion of string

        contents = new hardware::Byte[byteCount];

        clear();
    }


    Memory::Memory( const Memory& other) :
        lowerBound( other.lowerBound ),
        upperBound( other.upperBound ),
        byteCount( other.byteCount ),
        contents( new hardware::Byte[byteCount] )
    {
        memcpy( this->contents, other.contents, byteCount );
        memcpy( this->bankName, other.bankName, sizeof( this->bankName ));
    }

    Memory::Memory( Memory&& other) :
        lowerBound( other.lowerBound ),
        upperBound( other.upperBound ),
        byteCount( other.byteCount ),
        contents( other.contents )
    {
        other.contents = nullptr;
        memcpy( this->bankName, other.bankName, sizeof( this->bankName ));
    }


    // --- destructor
    Memory::~Memory()
    {
        delete[] contents;
    }

    // ===== Methods =====

    // Memory& Memory::operator=(const Memory& rhs )    // Copy assignment
    // {
    //     if ( this != &rhs )
    //     {
    //         delete[] this->contents;

    //         this->lowerBound = rhs.lowerBound;
    //         this->upperBound = rhs.upperBound;
    //         this->byteCount = rhs.byteCount;
    //         this->contents = new hardware::Byte[byteCount];
    //         memcpy( this->contents, rhs.contents, this->byteCount );
    //         memcpy( this->bankName, rhs.bankName, sizeof( this->bankName ) );
    //     }

    //     return *this;
    // }

    // Memory& Memory::operator=(Memory&& rhs )         // Move assignment
    // {
    //     if (this != &rhs)
    //     {
    //         this->lowerBound = rhs.lowerBound;
    //         this->upperBound = rhs.upperBound;
    //         this->byteCount = rhs.byteCount;
    //         this->contents = rhs.contents;
    //         memcpy( this->bankName, rhs.bankName, sizeof( this->bankName ) );

    //         // Force rhs to be unuseable
    //         rhs.lowerBound = 0;
    //         rhs.upperBound = 0;
    //         rhs.byteCount = 0;
    //         rhs.contents = nullptr;
    //     }
    // }


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
        // hardware::Byte value = contents[address];
        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << "   read( " << std::setfill('0') << std::setw(4) << (int)address << " ) = "
        //           // << " byte:  " << (int)contents[address]
        //           // << " MSB:  " << (int)contents[address + 1]
        //           << " Byte: " << (int)value << std::endl;
        // std::cout.unsetf(std::ios::basefield);

        return lowerBound <= address && address <= upperBound;
    }

    bool Memory::isInBounds(const size_t address) const
    {
        return isInBounds( hardware::Address((unsigned int)address) );
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




    // read a byte from memory
    hardware::Byte Memory::read(const hardware::Address &address) const
    {
        if (isInBounds(address))
        {
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
        if ( isInBounds( address ))
        {
            size_t msbOffset = address - lowerBound + 1;
            // handle wrap around
            msbOffset = msbOffset <= byteCount ? msbOffset : 0;
            size_t lsbOffset = address - lowerBound;

            hardware::Word addr(contents[msbOffset], contents[lsbOffset]);

            return addr;
        }

        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));

    }
    hardware::Address Memory::readAddress(const hardware::Address &address) const
    {
        if ( isInBounds( address ))
        {
            size_t msbOffset = address - lowerBound + 1;
            // handle wrap around
            msbOffset = msbOffset <= byteCount ? msbOffset : 0;
            size_t lsbOffset = address - lowerBound;

            hardware::Address addr(contents[msbOffset], contents[lsbOffset]);

            return addr;
        }

        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));
    }


    // write a byte to memory, returning the value written
    hardware::Byte Memory::write(const hardware::Address &address, const hardware::Byte value)
    {
        if ( isInBounds( address ))
        {
            return contents[address - lowerBound] = value;
        }

        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));
    }

    // 8000:9fff [20000]
    // writeWord( 9FFF, 1122 )
    // 9fff = 22
    // 8000 = 11
    // 9fff - 8000 -> 1fff
    // address: 9FFF
    // lsboffset = 9FFF - 9FFF -> 0
    // msbOffset = 9FFF + 1 - 8000 -> 1FFF + 1 -> 2000
    hardware::Word Memory::writeWord(const hardware::Address &address, const hardware::Word value)
    {
        if ( isInBounds( address ))
        {
            size_t msbOffset = address - lowerBound + 1;
            // handle wrap around
            msbOffset = msbOffset >= byteCount ? 0 : msbOffset;
            size_t lsbOffset = address - lowerBound;

            contents[lsbOffset] = value.lo;
            contents[msbOffset] = value.hi;

            return value;
        }

        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));
    }

    hardware::Address Memory::writeAddress(const hardware::Address& address, const hardware::Address& value)
    {
        if ( isInBounds( address ))
        {
            size_t msbOffset = address - lowerBound + 1;
            // handle wrap around
            msbOffset = msbOffset <= byteCount ? msbOffset : 0;
            size_t lsbOffset = address - lowerBound;

            contents[lsbOffset] = value.value.address.lo;
            contents[msbOffset] = value.value.address.hi;

            return value;
        }

        throw std::out_of_range(
            std::format("address {:#06x} not in block [{:#06x},{:#06x}]",
                        address.value.address.word,
                        lowerBound.value.address.word,
                        upperBound.value.address.word));
    }

    void Memory::showMemory(const hardware::Address& from, const int count, const unsigned char bytesPerLine) const
    {
        showMemory( from, count, bytesPerLine, "" );
    }
    void Memory::showMemory(const hardware::Address& from, const int count, const char *text) const
    {
        showMemory( from, count, 8, text );
    }
    void Memory::showMemory(const hardware::Address& from, const int count, const unsigned char bytesPerLine, const char *text) const
    {
        if ( !isInBounds( from ))
        {
            std::cout << "Address: " << from << " is not in range" << std::endl;

            return;
        }

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
            std::cout << " " << std::setw(2) << (int)contents[addr - lowerBound];
            ++addr;
            if (diff != 0 && ((diff % bytesPerLine) == 0))
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
