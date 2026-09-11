

#include "hardware.hpp"
#include "6502.hpp"

#include "AddressMode.hpp"
namespace m6502
{
    // ----- AddressMode -----
    hardware::Address CPU::AddressMode::execute()
    {
        // std::cout << name() << " ; " << mnemonic() << std::endl;
        // return hardware::Address(0xDEADBEEF);
        return hardware::Address(0xDEAD);
    }

    // ----- AddressModeUndefined -----
    hardware::Address CPU::AddressModeUndefined::execute()
    {
        std::cout << "AddressMode(Undefined): Not yet implemented" << std::endl;
        throw std::domain_error("AddressMode(Undefined)");
        // return hardware::Address(0xFADEFACE);
        return hardware::Address(0xFADE);
    }

    // ----- AddressModeZeroPage -----
    hardware::Address CPU::AddressModeZeroPage::execute()
    {
        AddressMode::execute();

        hardware::Byte zpOffset = cpu.addressSpace[cpu.registers.PC];
        cpu.registers.PC++;

        hardware::Address address(zpOffset);

        return address;
    }

    // ----- AddressModeZeroPageIndexedX -----
    hardware::Address CPU::AddressModeZeroPageIndexedX::execute()
    {
        AddressMode::execute();

        hardware::Byte zpOffset = cpu.addressSpace[cpu.registers.PC]; // operand
        cpu.registers.PC++;
        zpOffset += cpu.registers.X; // add offset

        hardware::Address address(zpOffset);

        return address;
    }

    // ----- AddressModeZeroPageIndexedY -----
    hardware::Address CPU::AddressModeZeroPageIndexedY::execute()
    {
        AddressMode::execute();

        hardware::Byte zpOffset = cpu.addressSpace[cpu.registers.PC]; // operand
        cpu.registers.PC++;

        hardware::Address address(zpOffset + cpu.registers.Y);

        return address;
    }

    // ----- AddressModeAbsolute -----
    hardware::Address CPU::AddressModeAbsolute::execute()
    {
        AddressMode::execute();
        hardware::Address absolute = cpu.addressSpace.readAddress(cpu.registers.PC);
        // cpu.decodePipeline().operand = cpu.addressSpace.read(absolute);
        // cpu.registers.PC.value.address += 2;
        cpu.registers.PC += 2;

        return absolute;
    }

    // ----- AddressModeRelative -----
    hardware::Address CPU::AddressModeRelative::execute()
    {
        std::cout << "AddressMode(Relative): Not yet implemented" << std::endl;
        // TODO return 'byte - offset'
        return AddressMode::execute();

        // AddressMode::execute();
        // hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
        // // cpu.decodePipeline().operand = cpu.addressSpace.read(absolute);
        // cpu.registers.PC.address += 2;

        // return absolute;
    }

    // ----- AddressModeAbsoluteIndexedX -----
    hardware::Address CPU::AddressModeAbsoluteIndexedX::execute()
    {
        // value = read( $nnnn + X)
        AddressMode::execute();

        hardware::Address absolute = cpu.addressSpace.readAddress(cpu.registers.PC);
        // cpu.registers.PC.value.address += 2;
        cpu.registers.PC += 2;
        hardware::Address address(absolute + cpu.registers.X);
        // cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.registers.X);

        return address;
    }

    // ----- AddressModeAbsoluteIndexedY -----
    hardware::Address CPU::AddressModeAbsoluteIndexedY::execute()
    {
        // value = read( $nnnn + Y)
        AddressMode::execute();
        hardware::Address absolute = cpu.addressSpace.readAddress(cpu.registers.PC);
        // cpu.registers.PC.value.address += 2;
        cpu.registers.PC += 2;
        hardware::Address address(absolute + cpu.registers.Y);

        return address;
    }

    // ----- AddressModeIndirect -----
    hardware::Address CPU::AddressModeIndirect::execute()
    {
        AddressMode::execute();
        hardware::Address indirectAddress = cpu.addressSpace.readAddress(cpu.registers.PC);
        cpu.registers.PC.value.address += 2;
        // cpu.registers.PC += 2;

        hardware::Address finalAddress = cpu.addressSpace.readAddress(indirectAddress);
        //        cpu.decodePipeline().operand = cpu.addressSpace.readWord(indirectAddress);

        return finalAddress;
    }

    // ----- AddressModeIndexedIndirectX -----
    // $nn: $1234
    //   X: $10
    //        ┌───────┐
    // $0070  │ $1234 │        $nn - points to a table of addresses ($1234)
    //        └───────┘         │
    //              ┌───────<───┘
    //        ┌───────┐
    // $1234  │ $8200 │         1st table entry
    //   :    └───────┘
    //   :    ┌───────┐
    // $1236  │ $8230 │         2nd table entry
    //   :    └───────┘
    //   :    ┌───────┐
    // $1244  │ $8260 │ ──>──┐  Xth entry in table  ----> $8260
    //        └───────┘      │
    //             ┌────<────┘
    //        ┌─────┐
    // $8260  │ $49 │
    //        └─────┘
    // ---------------------------------------------------------------------
    hardware::Address CPU::AddressModeIndexedIndirectX::execute()
    {
        // value = read( $nn + X )

        // X is an index into zero page
        // the 16 bit address is found at:
        // pcl: zp + x
        // pch: zp + x + 1
        // value = read( $nnnn + X)

        AddressMode::execute();

        // Base address in zeroPage
        hardware::Address zeroPageBase(cpu.addressSpace[cpu.registers.PC]);
        cpu.registers.PC++;
        // index from the base address
        hardware::Address zeroPageAddress = zeroPageBase + cpu.registers.X;
        // Get the address from the indexed address
        hardware::Address address = cpu.addressSpace.readAddress(zeroPageAddress);

        return address;
    }

    // ----- AddressModeIndirectIndexedY -----
    // $nn: $70
    //   Y: $10
    //        ┌───────┐
    // $0070  │ $1234 │        $nn - points to a table of addresses ($1234)
    //        └───────┘         │
    //                          │
    //              ┌───────<───┘
    //        ┌───────┐
    // $1234  │ $8200 │             1st address table entry
    //        └───────┘
    //        ┌───────┐
    // $1236  │ $8210 │             2nd address table entry
    //   :    └───────┘
    //   :        :
    //   :        :
    //   :    ┌───────┐
    // $1244  │ $8260 │ ──>──┐     Y-th entry in table
    //        └───────┘      │
    //             ┌─<────<──┘
    //        ┌─────┐
    // $8260  │ $49 │
    //        └─────┘
    // -----------------------------------------------------------------------
    hardware::Address CPU::AddressModeIndirectIndexedY::execute()
    {
        AddressMode::execute();
        // read( read( $nn ) | (read( $nn + 1) << 8) + Y )
        // indirect pointer (PC | (PC+1)<<8) + Y
        // value = read( $nn + X )

        // Base address in zeroPage
        hardware::Address zeroPageAddress(cpu.addressSpace[cpu.registers.PC]);
        cpu.registers.PC++;
        hardware::Address tableBase(cpu.addressSpace.readWord(zeroPageAddress));
        hardware::Address address(cpu.addressSpace.readWord(tableBase + cpu.registers.Y));

        return address;
    }

    // ----- AddressModeImmediate -----
    hardware::Address CPU::AddressModeImmediate::execute()
    {
        AddressMode::execute();

        hardware::Address address = cpu.registers.PC;
        cpu.registers.PC++;

        return address;
    }

    // ----- AddressModeImplied -----
    hardware::Address CPU::AddressModeImplied::execute()
    {
        AddressMode::execute();

        return cpu.registers.PC; // return address is always ignored.
        // return 0xFFFF; // return address is always ignored.
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
        // AddressMode::execute();

        // The memory address to write to is the current Stack Pointer
        // Stack pointer is modified (decremented) after the push
        // AddressMode returns the address to be acted upon
        hardware::StackAddress address(cpu.S());

        cpu.registers.S--; // TODO create a StackAddress class (fixed pch of 0x01)

        return address;
    }

    hardware::Address CPU::AddressModeStackPull::execute()
    {
        AddressMode::execute();

        // increment stack before calculating the address
        // cpu.registers.S++;
        // return cpu.registers.S & stackMask;
        // hardware::Address address(cpu.registers.S);
        // hardware::Address address(stackPage, cpu.registers.S.current());

        // return address;
        return ++cpu.registers.S;
    }

}
