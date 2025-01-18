

#ifndef HARDWARE_HPP
#define HARDWARE_HPP

namespace hardware
{
    // Size of General purpose registers and the data bus
    using Byte = unsigned char; // or uint8_t

    // Offset anywhere in the address spece or size of an address space
    using Word = unsigned short; // or uint16_t

    // Size of the address bus and correspondingly the Program Counter and stack pointer
    using Address = unsigned short; // or uint16_t
}

#endif // HARDWARE_HPP
