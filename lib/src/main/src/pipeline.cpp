

#include <iostream>
#include <iomanip>

#include "6502.hpp"
#include "InstructionSet.hpp"

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
        cpuInstruction = cpu._instructionUndefined;
    }

    /**
     * @brief Execute a single instruction cycle.
     *
     */
    void CPU::Pipeline::execute()
    {
        std::cout << ">>>>> pipeline: single instruction cycle <<<<<" << std::endl;

        dst = src = InstructionTarget::Undefined;

        fetchOpCode();
        AddressMode &addressMode = decodeAddressMode(opCode);
        decodeOperation(opCode);
        // fetchOperand(addressMode);
        evaluate();
        // cpu.decodePipeline().cpuInstruction->execute();
        std::cout << "<<<<< ---------------------------------- >>>>>" << std::endl;
    }

    /**
     * @brief Execute 'n' instruction cycles.
     *
     * @param numberOfInstructions the number of instruction cycles to execute.
     */
    void CPU::Pipeline::execute(int numberOfInstructions)
    {
        // std::cout << " pipeliine: execute 'n' instruction " << std::endl;

        for (int x = 0; x < numberOfInstructions; x++)
        {
            execute();
        }
        // TODO evaluate the instruction
    }

    void CPU::Pipeline::fetchOpCode()
    {
        // Reset pipeline from any prior instruction
        addressMode = cpu._addressModeUndefined;
        cpuInstruction = cpu._instructionUndefined;

        opCode.value = cpu.addressSpace.read(cpu.registers.PC++);
        // instruction = opcode;
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout << std::setfill('0') << std::setw(2) << " OpCode< " << (int)opCode.value << std::endl;
        std::cout.unsetf(std::ios::basefield);
    }

    CPU::AddressMode &CPU::Pipeline::decodeAddressMode(const OpCode opCode)
    {
        // std::cout << " Decode the addressingMode: ";

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " OpCode: " << std::setfill('0') << std::setw(2) << (int)opCode.value << std::endl;
        // // std::cout << " OpCode: " << std::setfill('0') << std::setw(2)
        // //           << opCode.memory.c << std::endl;
        // std::cout.unsetf(std::ios::basefield);
        // std::cout.setf(std::ios::oct, std::ios::basefield);
        // std::cout << "         "
        //           << " a: " << std::bitset<8>(opCode.value)
        //           << " a: " << std::bitset<3>(opCode.memory.a)
        //           << " b: " << std::bitset<3>(opCode.memory.b)
        //           << " c: " << std::bitset<2>(opCode.memory.c)
        //           << std::endl;
        // std::cout.unsetf(std::ios::basefield);

        // Decode an instruction byte which is decomposed into bit fields within a byte
        // format: aaabbbcc where aaa, bbb, cc represent groups of 2 or 3 bits.  Each
        // letter represents a single bit.
        // std::cout << "AddressMode: switch(b: " << std::bitset<3>(opCode.memory.b) << ") >";
        switch (opCode.memory.b) // 3 bits
        {
        case 0x000:                  // b(0)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00:                   // b(0) c(0)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(0) c(0) a(0)
                case 0b010: // b(0) c(0) a(2)
                case 0b011: // b(0) c(0) a(3)
                    //                    std::cout << "impl" << std::endl;
                    addressMode = cpu._addressModeImplied;
                    break;
                case 0b001: // b(0) c(0) a(1)
                    //                    std::cout << "abs" << std::endl;
                    addressMode = cpu._addressModeAbsolute;
                    break;
                case 0b100: // b(0) c(0) a(4)
                    // n/a
                    break;
                case 0b101: // b(0) c(0) a(5)
                case 0b110: // b(0) c(0) a(6)
                case 0b111: // b(0) c(0) a(7)
                    //                    std::cout << "# immediate" << std::endl;
                    addressMode = cpu._addressModeImmediate;
                    break;
                }
                break;
            case 0b01: // b(0) c(1)
                //                std::cout << "X,ind" << std::endl;
                addressMode = cpu._addressModeIndexedIndirectX;
                break;
            case 0b10:                   // b(0) c(2)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // c(2) a(0)
                case 0b001: // c(2) a(1)
                case 0b010: // c(2) a(2)
                case 0b011: // c(2) a(3)
                case 0b100: // c(2) a(4)
                    // n/a
                    break;
                case 0b101: // c(2) a(5)
                    //                    std::cout << "# immediate" << std::endl;
                    addressMode = cpu._addressModeImmediate;
                    break;
                case 0b110: // c(2) a(6)
                case 0b111: // c(2) a(7)
                    // n/a
                    break;
                }
                break;
            case 0b11: // b(0) c(3)
                break;
            }
            break;
        case 0x001: // b(1)
            // std::cout << " switch(c: " << std::bitset<2>(opCode.memory.c) << ")" << std::endl;
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00: // b(1) c(0)
                // std::cout << " switch(a: " << std::bitset<3>(opCode.memory.a) << ")" << std::endl;
                switch (opCode.memory.a) // 2 bits
                {
                case 0b000: // b(1) c(0) a(0)
                case 0b010: // b(1) c(0) a(2)
                case 0b011: // b(1) c(0) a(3)
                    // n/a
                    break;
                case 0b001: // b(1) c(0) a(1)
                case 0b100: // b(1) c(0) a(4)
                case 0b101: // b(1) c(0) a(5)
                case 0b110: // b(1) c(0) a(6)
                case 0b111: // b(1) c(0) a(7)
                    //                    std::cout << "zero page" << std::endl;
                    addressMode = cpu._addressModeZeroPage;
                    break;
                }
                break;
            case 0b01: // b(1) c(1)
            case 0b10: // b(1) c(2)
                //                std::cout << "zero page" << std::endl;
                addressMode = cpu._addressModeZeroPage;
                break;
            case 0b11: // b(1) c(3)
                // n/a
                break;
            }
            break;
        case 0b010:                  // b(2)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00: // b(2) c(0)
                       //                std::cout << "c(0) ";
                       //                std::cout << "impl" << std::endl;
                addressMode = cpu._addressModeImplied;
                break;
            case 0b01:                             // b(2) c(1)
                                                   //                std::cout << "c(1) ";
                switch ((unsigned)opCode.memory.a) // 3 bits
                {
                case 0b000: // 0    b(2) c(1) a(0)
                case 0b001: // 1    b(2) c(1) a(1)
                case 0b010: // 2    b(2) c(1) a(2)
                case 0b011: // 3    b(2) c(1) a(3)
                case 0b101: // 5    b(2) c(1) a(5)
                case 0b110: // 6    b(2) c(1) a(6)
                case 0b111: // 7    b(2) c(1) a(7)
                            //                    std::cout << "a(0,1,3,4,5,6,7) ";
                            //                    std::cout << "immediate #" << std::endl;
                    // addressModeKind = AddressModeKind::IMMEDIATE;
                    addressMode = cpu._addressModeImmediate;
                    // addressMode = new CPU::AddressModeImmediate();
                    // addressMode = new AddressMode::AddressModeImmediate();
                    // addressMode = new CPU::AddressMode::AddressModeImmediate();
                    break;
                case 0b100: // 4    b(2) c(1) a(4)
                            //                    std::cout << "a(4) ";
                    // n/a
                    break;
                default: //    b(2) c(1) a(?)
                         //                    std::cout << "a(?): " << (unsigned)opCode.memory.a;
                    // error
                    break;
                }
                break;
            case 0b10:                   //    b(2) c(2)
                                         //                std::cout << "c(2) ";
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(2) c(2) a(0)
                case 0b001: // b(2) c(2) a(1)
                case 0b010: // b(2) c(2) a(2)
                case 0b011: // b(2) c(2) a(3)
                    //                    std::cout << "A" << std::endl;
                    break;
                case 0b100: // b(2) c(2) a(4)
                case 0b101: // b(2) c(2) a(5)
                case 0b110: // b(2) c(2) a(6)
                case 0b111: // b(2) c(2) a(7)
                    //                    std::cout << "impl" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b11: // b(2) c(3)
                //                std::cout << "c(3) n/a ";
                // n/a
                break;
            }
            break;
        case 0b011:                  // b(3)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00:                   // b(3) c(0)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(3) c(0) a(0)
                    // n/a
                    break;
                case 0b001: // b(3) c(0) a(1)
                case 0b010: // b(3) c(0) a(2)
                case 0b011: // b(3) c(0) a(3)
                case 0b100: // b(3) c(0) a(4)
                case 0b101: // b(3) c(0) a(5)
                case 0b110: // b(3) c(0) a(6)
                case 0b111: // b(3) c(0) a(7)
                    //                    std::cout << "absolute" << std::endl;
                    addressMode = cpu._addressModeAbsolute;
                    break;
                default:
                    // n/a
                    // error
                    break;
                }
                break;
            case 0b01: // b(3) c(1)
            case 0b10: // b(3) c(2)
                //                std::cout << "absolute" << std::endl;
                addressMode = cpu._addressModeAbsolute;
                break;
            case 0b11: // b(3) c(3)
                // n/a
                break;
            default:
                // error
                break;
            }
            break;
        case 0b100:                  // b(4)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00: // b(4) c(0)
                //                std::cout << "rel" << std::endl;
                addressMode = cpu._addressModeRelative;
                break;
            case 0b01: // b(4) c(1)
                //                std::cout << "ind, y" << std::endl;
                addressMode = cpu._addressModeIndirectIndexedY;
                break;
            case 0b10: // b(3) c(2)
            case 0b11: // b(3) c(3)
                // n/a
                break;
            }
            break;
        case 0b101: // b(5)
            // std::cout << "switch(c: " << std::bitset<2>(opCode.memory.c) << ")" << std::endl;
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00:                   // b(5) c(0)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(5) c(0) a(0)
                case 0b001: // b(5) c(0) a(1)
                case 0b010: // b(5) c(0) a(2)
                case 0b011: // b(5) c(0) a(3)
                    break;
                case 0b100: // b(5) c(0) a(4)
                case 0b101: // b(5) c(0) a(5)
                            //                    std::cout << "zp,X  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedX;
                    break;
                case 0b110: // b(5) c(0) a(6)
                case 0b111: // b(5) c(0) a(7)
                    // n/a
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b01: // b(5) c(1)
                       //                std::cout << "zp,X  zero page - indexed" << std::endl;
                addressMode = cpu._addressModeZeroPageIndexedX;
                break;
            case 0b10:                   // b(5) c(2)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(5) c(2) a(0)
                case 0b001: // b(5) c(2) a(1)
                case 0b010: // b(5) c(2) a(2)
                case 0b011: // b(5) c(2) a(3)
                case 0b110: // b(5) c(2) a(6)
                case 0b111: // b(5) c(2) a(7)
                            //                    std::cout << "zp,X  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedX;
                    break;
                case 0b100: // b(5) c(2) a(4)
                case 0b101: // b(5) c(2) a(5)
                            //                    std::cout << "zp,Y  zero page - indexed" << std::endl;
                    addressMode = cpu._addressModeZeroPageIndexedY;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b11: // b(5) c(3)
                // n/a
                break;
            default:
                // error
                break;
            }
            break;
        case 0b110:                  // b(6)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00: // b(6) c(0)
                       //                std::cout << "impl" << std::endl;
                addressMode = cpu._addressModeImplied;
                break;
            case 0b01: // b(6) c(1)
                       //                std::cout << "abs, y" << std::endl;
                addressMode = cpu._addressModeAbsoluteIndexedY;
                break;
            case 0b10:                   // b(6) c(2)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(6) c(2) a(0)
                case 0b001: // b(6) c(2) a(1)
                case 0b010: // b(6) c(2) a(2)
                case 0b011: // b(6) c(2) a(3)
                case 0b110: // b(6) c(2) a(6)
                case 0b111: // b(6) c(2) a(7)
                    // n/a
                    break;
                case 0b100: // b(6) c(2) a(4)
                case 0b101: // b(6) c(2) a(5)
                            //                    std::cout << "impl" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b11: // b(6) c(3)
                break;
            }
            break;
        case 0b111:                  // b(7)
            switch (opCode.memory.c) // 2 bits
            {
            case 0b00:                   // b(7) c(0)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(7) c(0) a(0)
                case 0b001: // b(7) c(0) a(1)
                case 0b010: // b(7) c(0) a(2)
                case 0b011: // b(7) c(0) a(3)
                case 0b100: // b(7) c(0) a(4)
                case 0b110: // b(7) c(0) a(6)
                case 0b111: // b(7) c(0) a(7)
                    // n/a
                    break;
                case 0b101: // b(7) c(0) a(5)
                            //                    std::cout << "abs,X   absoluute indexed" << std::endl;
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b01: // b(7) c(1)
                       //                std::cout << "abs,X   absoluute indexed" << std::endl;
                addressMode = cpu._addressModeAbsoluteIndexedX;
                break;
            case 0b10:                   // b(7) c(2)
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(7) c(2) a(0)
                case 0b001: // b(7) c(2) a(1)
                case 0b010: // b(7) c(2) a(2)
                case 0b011: // b(7) c(2) a(3)
                case 0b110: // b(7) c(2) a(6)
                case 0b111: // b(7) c(2) a(7)
                            //                    std::cout << "abs,X   absolute indexed" << std::endl;
                    break;
                case 0b101: // b(7) c(2) a(5)
                            //                    std::cout << "abs,Y   absolute indexed" << std::endl;
                    break;
                case 0b100: // b(7) c(2) a(4)
                    // n/a
                    break;
                default:
                    // error
                    break;
                }
                break;
            case 0b11: // b(7) c(3)
                break;
            }
            break;
        default: // b(?)
            std::cout << " b is not found " << std::endl;
        }

        // std::cout << " addrMode: " << addressMode->mnemonic() << std::endl;
        // std::cout << "><><><><><><><><><" << std::endl;

        return *addressMode;
    }

    void CPU::Pipeline::decodeOperation(const OpCode opcode)
    {
        // std::cout << " Decode the operation: ";

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " OpCode: " << std::setfill('0') << std::setw(2) << (int)opCode.value << std::endl;
        // // std::cout << " OpCode: " << std::setfill('0') << std::setw(2)
        // //           << opCode.memory.c << std::endl;
        // std::cout.unsetf(std::ios::basefield);
        // std::cout.setf(std::ios::oct, std::ios::basefield);
        // std::cout << ".."
        //           << "--: " << std::bitset<8>(opCode.value)
        //           << " a: " << std::bitset<3>(opCode.memory.a)
        //           << " b: " << std::bitset<3>(opCode.memory.b)
        //           << " c: " << std::bitset<2>(opCode.memory.c)
        //           << std::endl;

        addressMode = cpu._addressModeUndefined;
        // Decode an instruction byte which is decomposed into bit fields within a byte
        // format: aaabbbcc where aaa, bbb, cc represent groups of 2 or 3 bits.  Each
        // letter represents a single bit.
        // std::cout << "Instruction: switch(c: " << std::bitset<2>(opCode.memory.c) << ") > ";

        // switch (opCode.memory.b) // 3 bits
        // {
        // case 0b000: // c(0) a(5) b(0)
        // case 0b001: // c(0) a(5) b(1)
        // case 0b010: // c(0) a(5) b(2)
        // case 0b011: // c(0) a(5) b(3)
        // case 0b100: // c(0) a(5) b(4)
        // case 0b101: // c(0) a(5) b(5)
        // case 0b110: // c(0) a(5) b(6)
        // case 0b111: // c(0) a(5) b(7)
        //     break;
        // }

        switch (opCode.memory.c) // 2 bits
        {
        case 0b00: // c(0)
        {
            // Instruction
            switch (opCode.memory.a) // 3 bits
            {
            case 0b000: // c(0) a(0)
            {
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(0) b(0) - BRK impl
                case 0b001: // c(0) a(0) b(1)
                    break;
                case 0b010: // c(0) a(0) b(2) - PHP impl
                    cpuInstruction = cpu._instructionStack;
                    dst = InstructionTarget::STACK; // TODO was memory
                    src = InstructionTarget::PSR;
                    break;
                case 0b011: // c(0) a(0) b(3)
                case 0b100: // c(0) a(0) b(4) - BPL rel
                case 0b101: // c(0) a(0) b(5)
                case 0b110: // c(0) a(0) b(6) - CLC impl
                case 0b111: // c(0) a(0) b(7)
                    // Illegal
                    break;
                }
            }
            break;
            case 0b001:                  // c(0) a(1) - BIT
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(1) b(0) - JSR abs
                case 0b001: // c(0) a(1) b(1) - BIT zpg
                case 0b010: // c(0) a(1) b(2) - PLP impl
                    cpuInstruction = cpu._instructionStack;
                    dst = InstructionTarget::PSR;
                    src = InstructionTarget::STACK; // TODO was memory
                    break;
                case 0b011: // c(0) a(1) b(3) - BIT abs
                case 0b100: // c(0) a(1) b(4) - BMI rel
                case 0b101: // c(0) a(1) b(5) - illegal
                case 0b110: // c(0) a(1) b(6) - SEC impl
                case 0b111: // c(0) a(1) b(7) - illegal
                    break;
                }
                break;
            case 0b010:                  // c(0) a(2) - JMP
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(2) b(0) - RTI impl
                    break;
                case 0b001: // c(0) a(2) b(1)   Illegal
                case 0b101: // c(0) a(2) b(5)   Illegal
                case 0b111: // c(0) a(2) b(7)   Illegal
                    break;
                case 0b010: // c(0) a(2) b(2) - PHA impl
                    cpuInstruction = cpu._instructionStack;
                    dst = InstructionTarget::STACK; // TODO was memory
                    src = InstructionTarget::A;
                    break;
                case 0b011: // c(0) a(2) b(3) - JMP abs
                    break;
                case 0b100: // c(0) a(2) b(4) - BVC rel
                    break;
                case 0b110: // c(0) a(2) b(6) - CLI impl
                    break;
                }
                break;
            case 0b011:                  // c(0) a(3) - JMP (abs)
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(3) b(0) - RTS impl
                case 0b001: // c(0) a(3) b(1)
                    break;
                case 0b010: // c(0) a(3) b(2) - PLA impl
                    cpuInstruction = cpu._instructionStack;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::STACK; // TODO was memory
                    break;
                case 0b011: // c(0) a(3) b(3) - JMP ind
                case 0b100: // c(0) a(3) b(4) - BVS rel
                case 0b101: // c(0) a(3) b(5)
                case 0b110: // c(0) a(3) b(6) - SEI impl
                case 0b111: // c(0) a(3) b(7)
                    // Illegal
                    break;
                }
                break;
            case 0b100: // c(0) a(4) - STY, DEY, BCC, TYA
            {
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(4) b(0)   Illegal
                case 0b111: // c(0) a(4) b(7)   Illegal
                    break;
                case 0b001: // c(0) a(4) b(1) - STY $nn
                case 0b011: // c(0) a(4) b(3) - STY $nnnn
                case 0b101: // c(0) a(4) b(5) - STY $nn,X
                    cpuInstruction = cpu._instructionStore;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::Y;
                    break;
                case 0b010: // c(0) a(4) b(2) - DEY impl
                    cpuInstruction = cpu._instructionDecrementY;
                    dst = InstructionTarget::Y;
                    src = InstructionTarget::Y;
                    break;
                case 0b100: // c(0) a(4) b(4) - BCC $nn
                    break;
                case 0b110: // c(0) a(4) b(6) - TYA impl
                    cpuInstruction = cpu._instructionTransferYtoA;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::Y;
                    break;
                }

                /*
                hardware::Byte &reg = cpu.registers.Y;
                auto source = [reg](void)
                { return reg; };
                // auto wsource = [source]() { return source(); };
                // hardware::Byte (*fs)(void) = wsource;
                std::function<hardware::Byte()> func = source;
                func();

                // hardware::Byte ddd = addressSpace
                auto dd = []( func )
                // auto dest = [](std::function<hardware::Byte()> func)
                { func(); };
                // setDest( valFromSrc() );
                std::function<void(std::function<hardware::Byte()>)> ass = dest;

                dest(func());

                void (*dp)(hardware::Byte v);
                // auto d = [](  (*sf)( hardware::Byte v )    )

                hardware::Byte (*am)(const hardware::Address adr);

                // void (*assign)( hardware::Byte (*am)( const hardware::Address adr ) );
                //      assigng       Destination                              source
                void (*assign)(void (*dp)(hardware::Byte v), hardware::Byte (*am)(const hardware::Address adr));

                assign();

                    am = memory::Memory::read;

                // auto lambda = []() { return 5; }
                // int (*source)()
                void (*assign)(){};

                // create lambda, capturing the 'Y' register....
                // auto source = [cpu.registers.Y] () { return cpu.registers.Y; }
                // auto destination = [](){ return 43; };
                // auto source = [cpuInstruction]( (f*) target, (f*) origin ) { return target( origin() ); };
                //        op     destination                source
                // void (*fn)( void (*setDest)( int i ), int (*getSrc)() ) {};
                // void (*op)( (*dd)(), int (*ss)()) = []( (*d)(int), int (*s)() ) { d( s() ); };
                // auto source = []() { return target( origin() ); };
                // auto vvv = source(); // call lambda
                // auto inst = []( (*s)source, (*d)destination );
                // instruction.execute( destination(), source() )
                */
                break;
            }
            case 0b101:                  // c(0) a(5) - LDY
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(5) b(0) - LDY #$nn
                case 0b001: // c(0) a(5) b(1) - LDY $nn
                case 0b011: // c(0) a(5) b(3) - LDY $nnnn
                case 0b101: // c(0) a(5) b(5) - LDY $nn,X
                case 0b111: // c(0) a(5) b(7) - LDY $nnnn,X
                    cpuInstruction = cpu._instructionLoadY;
                    dst = InstructionTarget::Y;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b010: // c(0) a(5) b(2) - TAY
                    cpuInstruction = cpu._instructionTransferAtoY;
                    dst = InstructionTarget::Y;
                    src = InstructionTarget::A;
                    break;
                case 0b100: // c(0) a(5) b(4) - BCS $nn
                case 0b110: // c(0) a(5) b(6) - CLV
                    break;
                }
                break;
            case 0b110:                  // c(0) a(6) - CPY, BNE, CLD
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(6) b(0) - CPY #
                case 0b001: // c(0) a(6) b(1) - CPY $nn
                case 0b011: // c(0) a(6) b(3) - CPY $nnnn
                    break;
                case 0b010: // c(0) a(6) b(2) - INY
                    cpuInstruction = cpu._instructionIncrementY;
                    dst = InstructionTarget::Y;
                    src = InstructionTarget::Y;
                    break;
                case 0b100: // c(0) a(6) b(4) - BNE $nn
                    break;
                case 0b101: // c(0) a(6) b(5)   Illegal
                case 0b111: // c(0) a(6) b(7)   Illegal
                    break;
                case 0b110: // c(0) a(6) b(6) - CLD
                    break;
                }
                break;
            case 0b111:                  // c(0) a(7) - CPX, INX, BEQ, SED
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(0) a(7) b(0) - CPX #
                case 0b001: // c(0) a(7) b(1) - CPX $nn
                case 0b011: // c(0) a(7) b(3) - CPX $nnnn
                    break;
                case 0b010: // c(0) a(7) b(2) - INX
                    cpuInstruction = cpu._instructionIncrementX;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::X;
                    break;
                case 0b100: // c(0) a(7) b(4) - BEQ $nn
                    break;
                case 0b101: // c(0) a(7) b(5)   Illegal
                case 0b111: // c(0) a(7) b(7)   Illegal
                    break;
                case 0b110: // c(0) a(7) b(6) - SED
                    break;
                }
                break;
            }

            // Addressing Mode
            switch (opCode.memory.b) // 3 bits
            {
            case 0b000: // c(0) b(0) - Immediate
                // TODO more modes
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(0) a(0)
                case 0b010: // b(0) a(2)
                case 0b011: // b(0) a(3)
                    addressMode = cpu._addressModeImmediate;
                    break;
                case 0b001: // b(0) a(1)
                    addressMode = cpu._addressModeImplied;
                    break;
                case 0b101: // b(0) a(5)
                case 0b110: // b(0) a(6)
                case 0b111: // b(0) a(7)
                    addressMode = cpu._addressModeImmediate;
                    break;
                case 0b100: // b(0) a(4)        // TODO verify this for illegal instruction
                    break;
                }
                break;
            case 0b001: // c(0) b(1) - ZeroPage
                addressMode = cpu._addressModeZeroPage;
                // TODO check illegal instructions at a(0,2,3)
                break;
            case 0b010: // c(0) b(2) - Implied

                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(3) a(0) - PHP
                    addressMode = cpu._addressModeStackPush;
                    break;
                case 0b001: // b(3) a(1) - PLP
                    addressMode = cpu._addressModeStackPull;
                    break;
                case 0b010: // b(3) a(2) - PHA
                    addressMode = cpu._addressModeStackPush;
                    break;
                case 0b011: // b(3) a(3) - PLA
                    addressMode = cpu._addressModeStackPull;
                    break;
                case 0b100: // b(3) a(4) - DEY
                case 0b101: // b(3) a(5) - TAY
                case 0b110: // b(3) a(6) - INY
                case 0b111: // b(3) a(7) - INX
                    addressMode = cpu._addressModeImplied;
                    break;
                }

                break;
            case 0b011: // c(0) b(3) - Absolute
                        // TODO more modes
                addressMode = cpu._addressModeAbsolute;
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(3) a(0)    // TODO  verify illegal instruction
                case 0b001: // b(3) a(1)
                case 0b010: // b(3) a(2)
                case 0b011: // b(3) a(3)
                case 0b100: // b(3) a(4)
                case 0b101: // b(3) a(5)
                case 0b110: // b(3) a(6)
                case 0b111: // b(3) a(7)
                    break;
                }
                break;
            case 0b100: // c(0) b(4) - rel
                addressMode = cpu._addressModeRelative;
                break;
            case 0b101:                  // c(0) b(5) - ZeroPage,X
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(3) a(0)    // TODO  verify illegal instruction
                case 0b001: // b(3) a(1)
                case 0b010: // b(3) a(2)
                case 0b011: // b(3) a(3)
                case 0b110: // b(3) a(6)
                case 0b111: // b(3) a(7)
                    break;
                case 0b100: // b(3) a(4)
                case 0b101: // b(3) a(5)
                    addressMode = cpu._addressModeZeroPageIndexedX;
                    break;
                }
                break;
            case 0b110: // c(0) b(6) - Implied
                addressMode = cpu._addressModeImplied;
                break;
            case 0b111:                  // c(0) b(7) - Absolute,X
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // b(3) a(0)    // TODO  verify illegal instruction
                case 0b001: // b(3) a(1)
                case 0b010: // b(3) a(2)
                case 0b011: // b(3) a(3)
                case 0b100: // b(3) a(4)
                case 0b110: // b(3) a(6)
                case 0b111: // b(3) a(7)
                    break;
                case 0b101: // b(3) a(5)
                    addressMode = cpu._addressModeAbsoluteIndexedX;
                    break;
                }
                break;
            }
            break;
        }
        case 0b01: // c(1)
        {
            // std::cout << "switch(a: " << std::bitset<3>(opCode.memory.a) << ")" << std::endl;
            // Instruction
            switch (opCode.memory.a) // 3 bits
            {
            case 0b000:                                   // c(1) a(0) - ORA
                cpuInstruction = cpu._instructionLogical; // TOOD need ORA
                dst = InstructionTarget::A;
                src = InstructionTarget::MEMORY;
                break;
            case 0b001:                                   // c(1) a(1) - AND
                cpuInstruction = cpu._instructionLogical; // TOOD need AND
                dst = InstructionTarget::A;
                src = InstructionTarget::MEMORY;
                break;
            case 0b010:                                   // c(1) a(2) - EOR
                cpuInstruction = cpu._instructionLogical; // TOOD need EOR
                dst = InstructionTarget::A;
                src = InstructionTarget::MEMORY;
                break;
            case 0b011: // c(1) a(3) - ADC
                break;
            case 0b100: // c(1) a(4) - STA
                // std::cout << "case 4 (" << std::bitset<3>(opCode.memory.a) << ")" << std::endl;
                // b(2) is not an instruction
                cpuInstruction = cpu._instructionStore;
                dst = InstructionTarget::MEMORY;
                src = InstructionTarget::A;
                break;
            case 0b101: // c(1) a(5) - LDA
                // std::cout << "case 5 (" << std::bitset<3>(opCode.memory.a) << ")" << std::endl;
                cpuInstruction = cpu._instructionLoadA; // TOOD need LDA
                dst = InstructionTarget::A;
                src = InstructionTarget::MEMORY;
                break;
            case 0b110: // c(1) a(6) - CMP
                break;
            case 0b111: // c(1) a(7) - SBC
                break;
            }

            // Addressing Mode
            // std::cout << "Address Mode: switch(b: " << std::bitset<3>(opCode.memory.b) << ") ";
            switch (opCode.memory.b) // 3 bits
            {
            case 0b000: // c(1) b(0) - (ZeroPage,X)
                addressMode = cpu._addressModeIndexedIndirectX;
                break;
            case 0b001: // c(1) b(1) - ZeroPage
                addressMode = cpu._addressModeZeroPage;
                break;
            case 0b010: // c(1) b(2) - #Immediate
                addressMode = cpu._addressModeImmediate;
                break;
            case 0b011: // c(1) b(3) - Absolute
                addressMode = cpu._addressModeAbsolute;
                break;
            case 0b100: // c(1) b(4) - (ZeroPage),Y
                addressMode = cpu._addressModeIndirectIndexedY;
                break;
            case 0b101:                                         // c(1) b(5) - (ZeroPage,X)
                addressMode = cpu._addressModeZeroPageIndexedX; // ???  AddressModeIndexedIndirectX
                break;
            case 0b110: // c(1) b(6) - Absolute,Y
                addressMode = cpu._addressModeAbsoluteIndexedY;
                break;
            case 0b111: // c(1) b(7) - Absolute,X
                addressMode = cpu._addressModeAbsoluteIndexedX;
                break;
            }
            break;
        }
        case 0b10: // c(2)
        {
            // Instruction
            switch (opCode.memory.a) // 3 bits
            {
            case 0b000: // c(2) a(0) - ASL   // TODO ASL
                    switch (opCode.memory.b) // 3 bits
                    {
                    case 0b000: // c(2) a(0) b(0)   Illegal (JAM)
                    case 0b100: // c(2) a(0) b(4)   Illegal (JAM)
                    case 0b110: // c(2) a(0) b(6)   Illegal (NOP impl)
                        break;
                    case 0b010: // c(2) a(1) b(2) - ASL A
                        cpuInstruction = cpu._instructionShiftLeft;
                        dst = InstructionTarget::A;
                        src = InstructionTarget::A;
                        break;
                    case 0b001: // c(2) a(0) b(1) - ASL $nn
                    case 0b011: // c(2) a(0) b(3) - ASL $nnnn
                    case 0b101: // c(2) a(0) b(5) - ASL $nn,X
                    case 0b111: // c(2) a(0) b(7) - ASL $nnnn,X
                        cpuInstruction = cpu._instructionShiftLeft;
                        dst = InstructionTarget::MEMORY;
                        src = InstructionTarget::MEMORY;
                        break;
                    }
                break;
            case 0b001:                  // c(2) a(1) - ROL
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(1) b(0)   Illegal (JAM)
                case 0b100: // c(2) a(1) b(4)   Illegal (JAM)
                case 0b110: // c(2) a(1) b(6)   Illegal (NOP impl)
                    break;
                case 0b010: // c(2) a(1) b(2) - ROL A
                    cpuInstruction = cpu._instructionRotateLeft;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::A;
                    break;
                case 0b001: // c(2) a(1) b(1) - ROL $nn
                case 0b011: // c(2) a(1) b(3) - ROL $nnnn
                case 0b101: // c(2) a(1) b(5) - ROL $nn,X
                case 0b111: // c(2) a(1) b(7) - ROL $nnnn,X
                    cpuInstruction = cpu._instructionRotateLeft;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::MEMORY;
                    break;
                }
                break;
            case 0b010: // c(2) a(2) - LSR

                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(2) b(0)   Illegal (JAM)
                case 0b100: // c(2) a(2) b(4)   Illegal (JAM)
                case 0b110: // c(2) a(2) b(6)   Illegal (NOP impl)
                    break;
                case 0b001: // c(2) a(2) b(1) - LSR $nn
                case 0b011: // c(2) a(2) b(3) - LSR $nnnn
                case 0b101: // c(2) a(2) b(5) - LSR $nn,X
                case 0b111: // c(2) a(2) b(7) - LSR $nnnn,X
                    cpuInstruction = cpu._instructionShiftRight;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b010: // c(2) a(2) b(2) - LSR A
                    cpuInstruction = cpu._instructionShiftRight;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::A;
                    break;
                }

                break;
            case 0b011:                  // c(2) a(3) - ROR
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(3) b(0)    Illegal (JAM)
                case 0b100: // c(2) a(3) b(4)    Illegal (JAM)
                case 0b110: // c(2) a(3) b(6)    IIlegal (NOP impl)
                    break;
                case 0b001: // c(2) a(3) b(1)  - ROR $nn
                case 0b011: // c(2) a(3) b(3)  - ROR $nnnn
                case 0b101: // c(2) a(3) b(5)  - ROR $nn,X
                case 0b111: // c(2) a(3) b(7)  - ROR $nnnn,X
                    cpuInstruction = cpu._instructionRotateRight;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b010: // c(2) a(3) b(2)  - ROR A
                    cpuInstruction = cpu._instructionRotateRight;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::A;
                    break;
                }
                break;
            case 0b100:                  // c(2) a(4) - STX
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(4) b(0) -
                case 0b100: // c(2) a(4) b(4) -
                case 0b111: // c(2) a(4) b(7) -  "
                    // n/a - Illegal
                    break;
                case 0b001: // c(2) a(4) b(1) -  "
                case 0b011: // c(2) a(4) b(3) -  "
                case 0b101: // c(2) a(4) b(5) -  "
                    cpuInstruction = cpu._instructionStore;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::X;
                    break;
                case 0b010: // c(2) a(4) b(2) - LSR
                    cpuInstruction = cpu._instructionTransferXtoA;
                    dst = InstructionTarget::A;
                    src = InstructionTarget::X;
                    break;
                case 0b110: // c(2) a(4) b(6) - DEC
                    cpuInstruction = cpu._instructionTransferXtoS;
                    dst = InstructionTarget::S;
                    src = InstructionTarget::X;
                    break;
                }
                break;
            case 0b101:                  // c(2) a(5) - LDX
                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(5) b(0) - LDX
                case 0b001: // c(2) a(5) b(1) -  "
                case 0b011: // c(2) a(5) b(3) -  "
                case 0b101: // c(2) a(5) b(5) -  "
                case 0b111: // c(2) a(5) b(7) -  "
                    cpuInstruction = cpu._instructionLoadX;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b010: // c(2) a(5) b(2) - LSR
                    cpuInstruction = cpu._instructionTransferAtoX;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::A;
                    break;
                case 0b100: // c(2) a(5) b(4) -
                            // n/a Illegal
                    break;
                case 0b110: // c(2) a(5) b(6) - DEC
                    cpuInstruction = cpu._instructionTransferStoX;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::S;
                    break;
                }
                break;
            case 0b110: // c(2) a(6) - DEC, DEX

                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(6) b(0)   Illegal
                case 0b100: // c(2) a(6) b(4)   Illegal
                case 0b110: // c(2) a(6) b(6)   Illegal
                    break;
                case 0b001: // c(2) a(6) b(1) - DEC
                case 0b011: // c(2) a(6) b(3) - DEC
                case 0b101: // c(2) a(6) b(5) - DEC
                case 0b111: // c(2) a(6) b(7) - DEC
                    cpuInstruction = cpu._instructionDecrement;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b010: // c(2) a(6) b(2) - DEX
                    cpuInstruction = cpu._instructionDecrementX;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::X;
                    break;
                }

                break;
            case 0b111: // c(2) a(7) - INC

                switch (opCode.memory.b) // 3 bits
                {
                case 0b000: // c(2) a(7) b(0)  Illegal
                case 0b100: // c(2) a(7) b(4)  Illegal
                case 0b110: // c(2) a(7) b(6)  Illegal
                    break;
                case 0b001: // c(2) a(7) b(1) - INC $nn
                case 0b010: // c(2) a(7) b(2) - NOP
                case 0b011: // c(2) a(7) b(3) - INC $nnnn
                case 0b101: // c(2) a(7) b(5) - INC $nn,X
                case 0b111: // c(2) a(7) b(7) - INC $nnnn,X
                    cpuInstruction = cpu._instructionIncrement;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::MEMORY;
                    break;
                }
                break;
            }

            // Addressing Mode
            switch (opCode.memory.b) // 3 bits
            {
            case 0b000: // c(2) b(0) - Immediate
                addressMode = cpu._addressModeImmediate;
                break;
            case 0b001: // c(2) b(1) - ZeroPage
                addressMode = cpu._addressModeZeroPage;
                break;
            case 0b010:                  // c(2) b(2) - Accumulator or Implied
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // c(2) b(2) a(0) - Accumulator
                case 0b001: // c(2) b(2) a(1)
                case 0b010: // c(2) b(2) a(2)
                case 0b011: // c(2) b(2) a(3)
                    addressMode = cpu._addressModeAccumulator;
                    break;
                case 0b100: // c(2) b(2) a(4) - Implied
                case 0b101: // c(2) b(2) a(5)
                case 0b110: // c(2) b(2) a(6)
                case 0b111: // c(2) b(2) a(7)
                    addressMode = cpu._addressModeImplied;
                    break;
                }
                break;
            case 0b011: // c(2) b(3) - Absolute
                addressMode = cpu._addressModeAbsolute;
                break;
            case 0b100: // c(2) b(4) - ZeroPage,X   (ZeroPage),Y
                break;
            case 0b101: // c(2) b(5) - ZeroPage,X or ZeroPage,Y
                addressMode = cpu._addressModeZeroPageIndexedX;
                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // c(2) b(5) a(0) - ASL   // TODO ASL
                    break;
                case 0b001: // c(2) b(5) a(1) - ROL
                    break;
                case 0b010: // c(2) b(5) a(2) - LSR
                    break;
                case 0b011: // c(2) b(5) a(3) - ROR
                    break;
                case 0b100: // c(2) a(4) - STX
                    addressMode = cpu._addressModeZeroPageIndexedY;
                    dst = InstructionTarget::MEMORY;
                    src = InstructionTarget::X;
                    break;
                case 0b101: // c(2) a(5) - LDX
                    addressMode = cpu._addressModeZeroPageIndexedY;
                    dst = InstructionTarget::X;
                    src = InstructionTarget::MEMORY;
                    break;
                case 0b110: // c(2) a(6) - DEC
                    break;
                case 0b111: // c(2) a(7) - INC
                    break;
                }
                break;
            case 0b110: // c(2) b(6) - Absolute,Y
                // a(4,5) implied a(0-3,6-7) illegal
                addressMode = cpu._addressModeImplied;
                break;
                break;
            case 0b111: // c(2) b(7) - Absolute,X

                switch (opCode.memory.a) // 3 bits
                {
                case 0b000: // c(2) b(7) a(0) - ASL  // TODO ASL
                case 0b001: // c(2) b(7) a(1) - ROL
                case 0b010: // c(2) b(7) a(2) - LSR
                case 0b011: // c(2) b(7) a(3) - ROR
                case 0b110: // c(2) b(7) a(6) - DEC
                case 0b111: // c(2) b(7) a(7) - INC
                    addressMode = cpu._addressModeAbsoluteIndexedX;
                    break;
                case 0b100: // c(2) b(4) a(4) - STX
                    //  n/a
                    break;
                case 0b101: // c(2) b(5) a(5) - LDX
                    addressMode = cpu._addressModeAbsoluteIndexedY;
                    break;
                }
                break;
            }
            break;
        }
        case 0b11: // c(3)
            break;
        }

        // No memory access opcode
        if (addressMode == cpu._addressModeUndefined)
        {
            // 80(illegal), A0(ldy), C0(cpy), E0(cpx)
            // LSB(0)
            // no instruction matched yet.
            switch (opCode.branch.x)
            {
            case 0b00: // flag : negative (BPL, BMI)
                break;
            case 0b01: // flag : overflow (BVC, BVS)
                break;
            case 0b10: // flag : carry (BCC, BCS)
                break;
            case 0b11: // flag : zero (BNE, BEQ)
                break;
            }
        }

        // remaining opcodes LSB of 0, 8, A
        // stack - PHP, PLP, PHA, PLA
        // TAY. TYA,
        // DEY, INY, INX
        // CLC, SEC, CLI, SEI, CLV, CLD, SET

        // cpuInstruction->execute();

        // std::cout << std::endl
        //           << "==== parsed: " << cpuInstruction->mnemonic() << " " << addressMode->mnemonic() << std::endl;

        // std::cout.unsetf(std::ios::basefield);
    }

    void CPU::Pipeline::fetchOperand(AddressMode &mode)
    {
        //        std::cout << "fetchOperand: "
        //                  << " mode( " << mode.mnemonic() << " )"
        //                  << " addressMode( " << addressMode->mnemonic() << " )";
        // mode.execute();
        // addressMode->execute();

        // int operand = -1;

        //        std::cout << " fetch operand "
        //                  << std::endl;
        ;

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " : 0x" << (int)cpu.decodePipeline().operand;
        // std::cout.unsetf(std::ios::basefield);
        // std::cout << " " << cpu.decodePipeline().operand << std::endl;
    }

    void CPU::Pipeline::evaluate()
    {
        std::cout << " evaluate instruction ";
        // cpu.decodePipeline().operand;
        // cpu.decodePipeline().cpuInstruction->evaluate();
        std::cout
            << cpu.decodePipeline().cpuInstruction->mnemonic()
            << "  "
            << cpu.decodePipeline().addressMode->mnemonic()
            << " ; "
            << cpu.decodePipeline().operand
            << std::endl;

        cpu.decodePipeline().cpuInstruction->execute(cpu.decodePipeline().dst, cpu.decodePipeline().src);
        // mode.execute();
        // // int operand = -1;

        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " : 0x" << (int)cpu.decodePipeline().operand;
        // std::cout.unsetf(std::ios::basefield);
        // std::cout << " " << cpu.decodePipeline().operand << std::endl;
    }

    void CPU::Pipeline::showAddressMode(const AddressMode &addressMode) const
    {
        // std::cout << addressMode.name() << "    " << addressMode.mnemonic() << std::endl;
    }

    void CPU::Pipeline::showPipeline() const
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << " opcode: " << std::setfill('0') << std::setw(2) << instruction << std::endl;
        std::cout << "Pipeline decode:: OpCode: "
                  << (int)opCode.value
                  << " : "
                  << cpuInstruction->mnemonic()
                  << "  "
                  << addressMode->mnemonic()
                  << " ~ "
                  << std::endl;
        std::cout.unsetf(std::ios::basefield);

        // std::cout << addressMode.name() << "    " << addressMode.mnemonic() << std::endl;

        // showAddressMode(*addressMode);
    }

    void CPU::Pipeline::reset(hardware::Address resetVector)
    {
        clear();

        // std::cout << "----- pipeline -----" << std::endl;
        // std::cout.setf(std::ios::hex, std::ios::basefield);
        // std::cout << std::setfill('0') << std::setw(4) << "RESET vector: " << (int)resetVector << " -> load PC with value found at the reset vector" << std::endl;

        cpu.registers.PC = cpu.addressSpace.readWord(resetVector); // jump to address

        // std::cout << "   - Load PC "
        //           << std::setfill('0') << std::setw(4) << (int)cpu.registers.PC
        //           << " from reset vector "
        //           << std::setfill('0') << std::setw(4) << (int)resetVector << std::endl;
        // std::cout << "PC: " << std::setfill('0') << std::setw(4) << (int)registers.PC << " reset vector" << std::endl;
        // std::cout.unsetf(std::ios::basefield);
    }

    void CPU::Pipeline::clear()
    {
        opCode.value = -1;
        operand = -1;
        // cpu.addressMode = _addressModeUndefined;
    }
}
