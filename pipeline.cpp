

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

    /**
     * @brief Execute a single instruction cycle.
     *
     */
    void CPU::Pipeline::execute()
    {
        std::cout << " execute instruction pipeline " << std::endl;

        // for (int x = 0; x < numberOfInstructions; x++)
        // {
        // Pseudo instruction pipeline....
        //  fetch opcode
        //  decode opcode
        //  fetch operand (use address mode)
        //  perform operation
        //  store result in destination
        fetchOpCode();
        AddressMode &addressMode = decodeAddressMode(opCode);
        decodeSource();
        decodeDestination();
        decodeOperation();
        fetchOperand(addressMode);
        // }
        // TODO evaluate the instruction
    }

    /**
     * @brief Execute 'n' instruction cycles.
     *
     * @param numberOfInstructions the number of instruction cycles to execute.
     */
    void CPU::Pipeline::execute(int numberOfInstructions)
    {
        std::cout << " execute instruction pipeline " << std::endl;

        for (int x = 0; x < numberOfInstructions; x++)
        {
            execute();
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

    CPU::AddressMode &CPU::Pipeline::decodeAddressMode(const OpCode opCode)
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
                    // addressModeKind = AddressModeKind::IMMEDIATE;
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
                    std::cout << "absolute" << std::endl;
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
                std::cout << "absolute" << std::endl;
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

        return *addressMode;
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

    void CPU::Pipeline::fetchOperand(AddressMode &mode)
    {
        mode.execute();
        // int operand = -1;

        std::cout << " fetch operand ";

        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << " : 0x" << (int)cpu.decodePipeline().operand;
        std::cout.unsetf(std::ios::basefield);
        std::cout << " " << cpu.decodePipeline().operand << std::endl;
        // cpu.model.registers.A = cpu.decodePipeline().operand; // TODO Temporary
    }

    // void CPU::Pipeline::showAddressMode(const AddressModeKind addressModeKind) const
    void CPU::Pipeline::showAddressMode(const AddressMode &addressMode) const
    {
        std::cout << addressMode.name() << "    " << addressMode.mnemonic() << std::endl;
    }

    void CPU::Pipeline::showPipeline() const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " opcode: " << std::setfill('0') << std::setw(2) << instruction << std::endl;
        std::cout << " OpCode: " << (int)opCode.value << " ";
        std::cout.unsetf(std::ios::basefield);

        showAddressMode(*addressMode);
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