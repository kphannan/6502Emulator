

#include "hardware.hpp"
#include "6502.hpp"

namespace m6502
{
    //    namespace CPU
    //    {

    // ----- AddressMode -----
    hardware::Address CPU::AddressMode::execute()
    {
        std::cout << name() << " ; " << mnemonic() << std::endl;
        return hardware::Address(0xDEADBEEF);
    }

    // ----- AddressModeUndefined -----
    hardware::Address CPU::AddressModeUndefined::execute()
    {
        std::cout << "AddressMode(Undefined): Not yet implemented" << std::endl;
        return hardware::Address(0xFADEFACE);
    }

    // ----- AddressModeZeroPage -----
    hardware::Address CPU::AddressModeZeroPage::execute()
    {
        AddressMode::execute();
        hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);

        cpu.decodePipeline().operand = cpu.addressSpace.read(0x0000 + zpOffset);

        // TODO return 'address'
        return zpOffset;
    }

    // ----- AddressModeZeroPageIndexedX -----
    hardware::Address CPU::AddressModeZeroPageIndexedX::execute()
    {
        AddressMode::execute();
        hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
        cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.registers.X);

        // TODO return 'address'
        return zpOffset + cpu.registers.X;
    }

    // ----- AddressModeZeroPageIndexedY -----
    hardware::Address CPU::AddressModeZeroPageIndexedY::execute()
    {
        AddressMode::execute();
        hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
        cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.registers.Y);

        // TODO return 'address'
        return zpOffset + cpu.registers.Y;
    }

    // ----- AddressModeAbsolute -----
    hardware::Address CPU::AddressModeAbsolute::execute()
    {
        AddressMode::execute();
        hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
        cpu.decodePipeline().operand = cpu.addressSpace.read(absolute);
        cpu.registers.PC += 2;

        // TODO return 'address'
        return absolute;
    }

    // ----- AddressModeAbsoluteIndexedX -----
    hardware::Address CPU::AddressModeAbsoluteIndexedX::execute()
    {
        // value = read( $nnnn + X)
        AddressMode::execute();
        hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
        cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.registers.X);
        cpu.registers.PC += 2;

        // TODO return 'address'
        return absolute + cpu.registers.X;
    }

    // ----- AddressModeAbsoluteIndexedY -----
    hardware::Address CPU::AddressModeAbsoluteIndexedY::execute()
    {
        // value = read( $nnnn + Y)
        AddressMode::execute();
        hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
        cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.registers.Y);
        cpu.registers.PC += 2;

        // TODO return 'address'
        return absolute + cpu.registers.Y;
    }

    // ----- AddressModeIndirect -----
    hardware::Address CPU::AddressModeIndirect::execute()
    {
        AddressMode::execute();
        hardware::Address indirectAddress = cpu.addressSpace.readWord(cpu.registers.PC++);

        cpu.decodePipeline().operand = cpu.addressSpace.readWord(indirectAddress);

        // TODO return 'address'
        return indirectAddress;
    }

    // ----- AddressModeIndexedIndirectX -----
    hardware::Address CPU::AddressModeIndexedIndirectX::execute()
    {
        // value = read( $nn + X )
        AddressMode::execute();
        hardware::Address zeroPageAddress = cpu.addressSpace.read(cpu.registers.PC++);
        hardware::Address address = cpu.addressSpace.readWord(zeroPageAddress + cpu.registers.X);
        cpu.decodePipeline().operand = cpu.addressSpace.read(address);

        // TODO return 'address'
        return address;
    }

    // ----- AddressModeIndirectIndexedY -----
    hardware::Address CPU::AddressModeIndirectIndexedY::execute()
    {
        AddressMode::execute();
        // read( read( $nn ) | (read( $nn + 1) << 8) + Y )
        // indirect pointer (PC | (PC+1)<<8) + Y
        // value = read( $nn + X )
        hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
        hardware::Address address = cpu.addressSpace.readWord(zpOffset);
        address += cpu.registers.Y;
        cpu.decodePipeline().operand = cpu.addressSpace.read(address);

        // TODO return 'address'
        return address;
    }

    // ----- AddressModeImmediate -----
    hardware::Address CPU::AddressModeImmediate::execute()
    {
        AddressMode::execute();

        // cpu.decodePipeline().operand = cpu.addressSpace.read(cpu.registers.PC++);

        // // TODO return 'byte - literal'
        // return cpu.addressSpace.read(cpu.registers.PC - 1); // TODO watch the PC value when opeand fetch is removed from this method
        return cpu.registers.PC++;
    }

    // ----- AddressModeImplied -----
    hardware::Address CPU::AddressModeImplied::execute()
    {
        AddressMode::execute();

        return 0xFFFF;      // return address is always ignored.
    }

    // ----- AddressModeStack -----
    hardware::Address CPU::AddressModeStack::execute()
    {
        return AddressMode::execute();

        // decrement stack after calculating the address
//        return (cpu.registers.S-- & stackMask) + stackPage;
    }

    hardware::Address CPU::AddressModeStackPush::execute()
    {
        AddressMode::execute();

        // decrement stack after calculating the address
        return (cpu.registers.S-- & stackMask) + stackPage;
    }

    hardware::Address CPU::AddressModeStackPull::execute()
    {
        AddressMode::execute();

        // increment stack before calculating the address
        cpu.registers.S++;
        return (cpu.registers.S & stackMask) + stackPage;
    }



    //    }
}
