

#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "AddressMode.hpp"

namespace m6502
{

    /**
     * @brief Implementation of the Instruction fetch, decode and execute
     * pipeliine within a 6502 Central Processing Unit (CPU).
     *
     */

    /**
     * @brief Instantiate an instance of Pipliine, connecting it to the CPU
     * that contains it.
     *
     */
    CPU::Pipeline::Pipeline(CPU &processor)
        : cpu(processor)
    {
        addressMode = cpu._addressModeUndefined;
    }

    void CPU::Pipeline::execute(int numberOfInstructions)
    {
        std::cout << " execute instruction pipeline " << std::endl;

        for (int x = 0; x < numberOfInstructions; x++)
        {
            // Pseudo instruction pipeline....
            //  fetch opcode
            //  decode opcode
            //  fetch operand (use address mode)
            //  perform operation
            //  store result in destination
            fetchOpCode();
            AddressModeKind addressModeKind = decodeAddressMode(opCode);
            decodeSource();
            decodeDestination();
            decodeOperation();
            fetchOperand(addressModeKind);
        }
        // TODO evaluate the instruction
    }

    void CPU::Pipeline::fetchOpCode()
    {
        opCode.value = cpu.addressSpace.read(cpu.model.registers.PC++);
        // instruction = opcode;
        std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " opcode: " << std::setfill('0') << std::setw(2) << instruction << std::endl;
        std::cout << " OpCode< " << (int)opCode.value << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

    CPU::AddressModeKind CPU::Pipeline::decodeAddressMode(const OpCode opCode)
    {
        std::cout << " Decode the addressingMode: ";

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " OpCode: " << std::setfill('0') << std::setw(2) << (int)opCode.value << std::endl;
        // // std::cout << " OpCode: " << std::setfill('0') << std::setw(2)
        // //           << opCode.bits.c << std::endl;
        // std::cout.unsetf(std::ios::basefield);
        // std::cout.setf(std::ios::oct, std::ios::basefield);
        std::cout << "         "
                  << " a: " << std::bitset<8>(opCode.value)
                  << " a: " << std::bitset<3>(opCode.bits.a)
                  << " b: " << std::bitset<3>(opCode.bits.b)
                  << " c: " << std::bitset<2>(opCode.bits.c)
                  << std::endl;
        std::cout.unsetf(std::ios::basefield);

        addressMode = cpu._addressModeUndefined;
        // Decode an instruction byte which is decomposed into bit fields within a byte
        // format: aaabbbcc where aaa, bbb, cc represent groups of 2 or 3 bits.  Each
        // letter represents a single bit.
        switch (opCode.bits.b) // 3 bits
        {
        case 00:                   // b(0)
            switch (opCode.bits.c) // 2 bits
            {
            case 0:
                switch (opCode.bits.a) // 3 bits
                {
                case 0:
                case 2:
                case 3:
                    std::cout << "impl" << std::endl;
                    addressMode = cpu._addressModeImplicit;
                    break;
                case 1:
                    std::cout << "abs" << std::endl;
                    addressMode = cpu._addressModeAbsolute;
                    break;
                case 4:
                    // n/a
                    break;
                case 5:
                case 6:
                case 7:
                    std::cout << "# immediate" << std::endl;
                    addressMode = cpu._addressModeImmediate;
                    break;
                }
                break;
            case 1:
                std::cout << "X,ind" << std::endl;
                addressMode = cpu._addressModeIndexedIndirectX;
                break;
            case 2:
                switch (opCode.bits.b) // 3 bits
                {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    // n/a
                    break;
                case 5:
                    std::cout << "# immediate" << std::endl;
                    addressMode = cpu._addressModeImmediate;
                    break;
                case 6:
                case 7:
                    // n/a
                    break;
                }
                break;
            case 3:
                break;
            }
            break;
        case 01:                   // b(1)
            switch (opCode.bits.c) // 2 bits
            {
            case 0:
                switch (opCode.bits.a) // 2 bits
                {
                case 0:
                case 2:
                case 3:
                    // n/a
                    break;
                case 1:
                case 4:
                case 5:
                case 6:
                case 7:
                    std::cout << "zero page" << std::endl;
                    break;
                }
                break;
            case 1:
            case 2:
                std::cout << "zero page" << std::endl;
                break;
            case 3:
                // n/a
                break;
            }
            break;
        case 02:                   // b(2)
            switch (opCode.bits.c) // 2 bits
            {
            case 0: // b(2) c(0)
                std::cout << "c(0) ";
                std::cout << "impl" << std::endl;
                addressMode = cpu._addressModeImplicit;
                break;
            case 1: // b(2) c(1)
                std::cout << "c(1) ";
                switch ((unsigned)opCode.bits.a) // 3 bits
                {
                case 0b000: // 0    b(2) c(1) a(0)
                case 0b001: // 1    b(2) c(1) a(1)
                case 0b010: // 2    b(2) c(1) a(2)
                case 0b011: // 3    b(2) c(1) a(3)
                case 0b101: // 5    b(2) c(1) a(5)
                case 0b110: // 6    b(2) c(1) a(6)
                case 0b111: // 7    b(2) c(1) a(7)
                    std::cout << "a(0,1,3,4,5,6,7) ";
                    std::cout << "immediate #" << std::endl;
                    addressModeKind = AddressModeKind::IMMEDIATE;
                    addressMode = cpu._addressModeImmediate;
                    // addressMode = new CPU::AddressModeImmediate();
                    // addressMode = new AddressMode::AddressModeImmediate();
                    // addressMode = new CPU::AddressMode::AddressModeImmediate();
                    break;
                case 0b100: // 4    b(2) c(1) a(4)
                    std::cout << "a(4) ";
                    // n/a
                    break;
                default: //    b(2) c(1) a(?)
                    std::cout << "a(?): " << (unsigned)opCode.bits.a;
                    // error
                    break;
                }
                break;
            case 2: //    b(2) c(2)
                std::cout << "c(2) ";
                switch (opCode.bits.a) // 3 bits
                {
                case 0:
                case 1:
                case 2:
                case 3:
                    std::cout << "A" << std::endl;
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    std::cout << "impl" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 3:
                std::cout << "c(3) n/a ";
                // n/a
                break;
            }
            break;
        case 03:                   // b(3)
            switch (opCode.bits.c) // 2 bits
            {
            case 0:
                switch (opCode.bits.a) // 3 bits
                {
                case 0:
                    // n/a
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    std::cout << "absoluute" << std::endl;
                    addressMode = cpu._addressModeAbsolute;
                    break;
                default:
                    // n/a
                    // error
                    break;
                }
                break;
            case 1:
            case 2:
                std::cout << "absoluute" << std::endl;
                addressMode = cpu._addressModeAbsolute;
                break;
            case 3:
                // n/a
                break;
            default:
                // error
                break;
            }
            break;
        case 04:                   // b(4)
            switch (opCode.bits.c) // 2 bits
            {
            case 00:
                std::cout << "rel" << std::endl;
                addressMode = cpu._addressModeRelative;
                break;
            case 01:
                std::cout << "ind, y" << std::endl;
                addressMode = cpu._addressModeIndirectIndexedY;
                break;
            case 02:
            case 03:
                // n/a
                break;
            }
            break;
        case 05:                   // b(5)
            switch (opCode.bits.c) // 2 bits
            {
            case 0:                    // b(5) c(0)
                switch (opCode.bits.a) // 3 bits
                {
                case 0: // b(5) c(0) a(0)
                case 1: // b(5) c(0) a(1)
                case 2: // b(5) c(0) a(2)
                case 3: // b(5) c(0) a(3)
                    break;
                case 4: // b(5) c(0) a(4)
                case 5: // b(5) c(0) a(5)
                    std::cout << "zp,X  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedX;
                    break;
                case 6: // b(5) c(0) a(6)
                case 7: // b(5) c(0) a(7)
                    // n/a
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 1: // b(5) c(1)
                std::cout << "zp,X  zero page - indexed" << std::endl;
                addressMode = cpu._addressModeZeroPageIndexedX;
                break;
            case 2:                    // b(5) c(2)
                switch (opCode.bits.a) // 3 bits
                {
                case 0: // b(5) c(2) a(0)
                case 1: // b(5) c(2) a(1)
                case 2: // b(5) c(2) a(2)
                case 3: // b(5) c(2) a(3)
                case 6: // b(5) c(2) a(6)
                case 7: // b(5) c(2) a(7)
                    std::cout << "zp,X  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedX;
                    break;
                case 4: // b(5) c(2) a(4)
                case 5: // b(5) c(2) a(5)
                    std::cout << "zp,Y  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedY;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 3:
                // n/a
                break;
            default:
                // error
                break;
            }
            break;
        case 06:                   // b(6)
            switch (opCode.bits.c) // 2 bits
            {
            case 00: // b(6) c(0)
                std::cout << "impl" << std::endl;
                addressMode = cpu._addressModeImplicit;
                break;
            case 01: // b(6) c(1)
                std::cout << "abs, y" << std::endl;
                addressMode = cpu._addressModeAbsoluteIndexedY;
                break;
            case 02:                   // b(6) c(2)
                switch (opCode.bits.a) // 3 bits
                {
                case 0: // b(6) c(2) a(0)
                case 1: // b(6) c(2) a(1)
                case 2: // b(6) c(2) a(2)
                case 3: // b(6) c(2) a(3)
                case 6: // b(6) c(2) a(6)
                case 7: // b(6) c(2) a(7)
                    // n/a
                    break;
                case 4: // b(6) c(2) a(4)
                case 5: // b(6) c(2) a(5)
                    std::cout << "impl" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 03: // b(6) c(3)
                break;
            }
            break;
        case 07:                   // b(7)
            switch (opCode.bits.c) // 2 bits
            {
            case 00:                   // b(7) c(0)
                switch (opCode.bits.a) // 3 bits
                {
                case 0: // b(7) c(0) a(0)
                case 1: // b(7) c(0) a(1)
                case 2: // b(7) c(0) a(2)
                case 3: // b(7) c(0) a(3)
                case 4: // b(7) c(0) a(4)
                case 6: // b(7) c(0) a(6)
                case 7: // b(7) c(0) a(7)
                    // n/a
                    break;
                case 5: // b(7) c(0) a(5)
                    std::cout << "abs,X   absoluute indexed" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 01: // b(7) c(1)
                std::cout << "abs,X   absoluute indexed" << std::endl;
                addressMode = cpu._addressModeAbsoluteIndexedX;
                break;
            case 02:                   // b(7) c(2)
                switch (opCode.bits.a) // 3 bits
                {
                case 0: // b(7) c(2) a(0)
                case 1: // b(7) c(2) a(1)
                case 2: // b(7) c(2) a(2)
                case 3: // b(7) c(2) a(3)
                case 6: // b(7) c(2) a(6)
                case 7: // b(7) c(2) a(7)
                    std::cout << "abs,X   absoluute indexed" << std::endl;
                    break;
                case 5: // b(7) c(2) a(5)
                    std::cout << "abs,Y   absoluute indexed" << std::endl;
                    break;
                case 4: // b(7) c(2) a(4)
                    // n/a
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 03: // b(7) c(3)
                break;
            }
            break;
        default: // b(?)
            std::cout << " b is not found " << std::endl;
        }

        // showAddressMode(addressMode);
        // addressMode->execute();

        return addressModeKind;
    }

    void CPU::Pipeline::decodeSource()
    {
    }

    void CPU::Pipeline::decodeDestination()
    {
    }

    void CPU::Pipeline::decodeOperation()
    {
    }

    void CPU::Pipeline::fetchOperand(const AddressModeKind addressModeKind)
    {
        addressMode->execute();
        int operand = -1;

        std::cout << " fetch operand ";
        // operand = addressSpace.read(address++);
        // model.registers.A = memory.read(address++);
        // std::cout << " operand: " << operand << std::endl;
        // std::cout << " implement fetch opcode based on the addressing mode of the instruction" << std::end;

        /*
        switch (addressModeKind)
        {
        case AddressModeKind::IMPLICIT: // Implicit
        {
            std::cout << "impl    OPC" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::ACCUMULATOR: // Accumulator         A
        {
            std::cout << "A       OPC A" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::IMMEDIATE: // Immediate           #$nn
        {
            std::cout << "#       OPC #$BB";
            operand = cpu.addressSpace.read(cpu.model.registers.PC++);
            // std::cout << "#       OPC #$" << std::cout.setf(std::ios::hex) << operand;

            // std::cout.setf(std::ios::hex, std::ios::basefield);
            // std::cout << "#       OPC #$" << std::setfill('0') << std::setw(2) << operand << "   ";
            // std::cout.unsetf(std::ios::basefield);

            break;
        }
        case AddressModeKind::ZERO_PAGE: // Zero Page           $nn        LO bits 4,5,6
        {
            // operand is a (byte) offset into zero page
            std::cout << "zpg     OPC $LL" << std::endl;
            operand = cpu.addressSpace.read(cpu.model.registers.PC & 0x00FF);
            cpu.model.registers.PC++;
            break;
        }
        case AddressModeKind::ZERO_PAGE_X: // Zero Page, X        $nn, X     LO bits 4,5,6
        {
            std::cout << "zpg,X   OPC $LL,X" << " not implemented" << std::endl;
            // read from page zero
            // address to read: operand + X register
            // hardware::Byte baseAddress = 0x0043;
            // long baseAddress = 0x0043;
            hardware::Byte baseAddress = cpu.addressSpace.read(cpu.model.registers.PC++);
            hardware::Address operandAddress = baseAddress + cpu.model.registers.X;
            operand = cpu.addressSpace.read(operandAddress);
            break;
        }
        case AddressModeKind::ZERO_PAGE_Y: // Zero Page, Y        $nn, Y     LO bits 4,5,6
        {
            std::cout << "zpg,Y   OPC $LL,Y" << " not implemented" << std::endl;
            // read from page zero
            // address to read: operand + X register
            hardware::Byte baseAddress = cpu.addressSpace.read(cpu.model.registers.PC++);
            hardware::Address operandAddress = baseAddress + cpu.model.registers.Y;
            operand = cpu.addressSpace.read(operandAddress);
            break;
        }
        case AddressModeKind::RELATIVE: // Relative            $nnnn
        {
            std::cout << "rel     OPC $BB" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::ABSOLUTE: // Absolute            $nnnn
        {
            std::cout << "abs     OPC $LLHH" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::ABSOLUTE_X: // Absolute, X         $nnnn, X
        {
            std::cout << "abs,X   OPC $LLHH,X" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::ABSOLUTE_Y: // Absolute, Y         $nnnn, Y
        {
            std::cout << "abs,Y   OPC $LLHH,Y" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::INDIRECT: // Indirect            ($nnnn)
        {
            std::cout << "ind     OPC ($LLHH)" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::INDEXED_INDIRECT_X: // X Indexed Indirect  ($nn, X)   LO bit 1
        {
            std::cout << "X,ind   OPC ($LL,X)" << " not implemented" << std::endl;
            break;
        }
        case AddressModeKind::INDIRECT_INDEXED_Y: // Y Indirect Indexed  ($nn), Y   LO bit 1
        {
            std::cout << "ind,Y   OPC ($LL),Y" << " not implemented" << std::endl;
            break;
        }

        default:
        {
            std::cout << "Unknown address mode" << std::endl;
            break;
        }
        }
        */

        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << " : 0x" << (int)operand;
        std::cout.unsetf(std::ios::basefield);
        std::cout << " " << operand << std::endl;
    }

    void CPU::Pipeline::showAddressMode(const AddressModeKind addressMode) const
    {
        switch (addressMode)
        {
        case AddressModeKind::IMPLICIT: // Implicit
            std::cout << "impl    OPC" << std::endl;
            break;
        case AddressModeKind::ACCUMULATOR: // Accumulator         A
            std::cout << "A       OPC A" << std::endl;
            break;
        case AddressModeKind::IMMEDIATE: // Immediate           #$nn
            std::cout << "#       OPC #$BB" << std::endl;
            break;
        case AddressModeKind::ZERO_PAGE: // Zero Page           $nn        LO bits 4,5,6
            std::cout << "zpg     OPC $LL" << std::endl;
            break;
        case AddressModeKind::ZERO_PAGE_X: // Zero Page, X        $nn, X     LO bits 4,5,6
            std::cout << "zpg,X   OPC $LL,X" << std::endl;
            break;
        case AddressModeKind::ZERO_PAGE_Y: // Zero Page, Y        $nn, Y     LO bits 4,5,6
            std::cout << "zpg,Y   OPC $LL,Y" << std::endl;
            break;
        case AddressModeKind::RELATIVE: // Relative            $nnnn
            std::cout << "rel     OPC $BB" << std::endl;
            break;
        case AddressModeKind::ABSOLUTE: // Absolute            $nnnn
            std::cout << "abs     OPC $LLHH" << std::endl;
            break;
        case AddressModeKind::ABSOLUTE_X: // Absolute, X         $nnnn, X
            std::cout << "abs,X   OPC $LLHH,X" << std::endl;
            break;
        case AddressModeKind::ABSOLUTE_Y: // Absolute, Y         $nnnn, Y
            std::cout << "abs,Y   OPC $LLHH,Y" << std::endl;
            break;
        case AddressModeKind::INDIRECT: // Indirect            ($nnnn)
            std::cout << "ind     OPC ($LLHH)" << std::endl;
            break;
        case AddressModeKind::INDEXED_INDIRECT_X: // X Indexed Indirect  ($nn, X)   LO bit 1
            std::cout << "X,ind   OPC ($LL,X)" << std::endl;
            break;
        case AddressModeKind::INDIRECT_INDEXED_Y: // Y Indirect Indexed  ($nn), Y   LO bit 1
            std::cout << "ind,Y   OPC ($LL),Y" << std::endl;
            break;

        default:
            std::cout << "Unknown address mode" << std::endl;
            break;
        }
    }

    void CPU::Pipeline::showPipeline() const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " opcode: " << std::setfill('0') << std::setw(2) << instruction << std::endl;
        std::cout << " OpCode: " << (int)opCode.value << " ";
        std::cout.unsetf(std::ios::basefield);

        showAddressMode(addressModeKind);
    }

    void CPU::Pipeline::reset(hardware::Address resetVector)
    {
        clear();
        cpu.model.registers.PC = cpu.addressSpace.readWord(resetVector); // jump to address

        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << "RESET - Load PC " << std::setfill('0') << std::setw(4) << (int)cpu.model.registers.PC << " from reset vector " << (int)resetVector << std::endl;
        // std::cout << "PC: " << std::setfill('0') << std::setw(4) << (int)model.registers.PC << " reset vector" << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

    void CPU::Pipeline::clear()
    {
        opCode.value = -1;
        operand = -1;
        // cpu.addressMode = _addressModeUndefined;
    }
}