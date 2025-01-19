

#ifndef ADDRESSMODE_HPP
#define ADDRESSMODE_HPP

#include <iostream>
#include <iomanip>

#include "6502.hpp"

namespace m6502
{
    // Addressing Modes

    /**
        // Instruction code chart
        // https://www.masswerk.at/6502/6502_instruction_set.html
        IMPLICIT,           // Implicit
        ACCUMULATOR,        // Accumulator         A                            4.6 Accumulator
        IMMEDIATE,          // Immediate           #$nn                         4.7 Immediate
        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6     4.11 Zero Page
        ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6     4.13 Zero Page Indexed with X
        ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6     4.14 Zero Page Indexed with Y
        RELATIVE,           // Relative            $nn                          4.9 Program Counter Relative
        ABSOLUTE,           // Absolute            $nnnn
        ABSOLUTE_X,         // Absolute, X         $nnnn, X                     4.3 AbsoluteIndexed with X
        ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y                     4.4 AbsoluteIndexed with Y
        INDIRECT,           // Indirect            ($nnnn)                      4.5 Absolute Indirect
        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
    */

    class CPU;

    class CPU::AddressMode
    {
        // Constructors
    protected:
        AddressMode(CPU &cpu, const char *name, const char *mnemonic) : cpu(cpu), modeName(name), modeMnemonic(mnemonic) {}

        // Fields
    private:
        const char *modeName;
        const char *modeMnemonic;

    protected:
        CPU &cpu;

        // Methods
    public:
        const char *name() const { return modeName; }
        const char *mnemonic() const { return modeMnemonic; }
        virtual hardware::Address execute()
        {
            std::cout << name() << " ; " << mnemonic() << std::endl;
            return hardware::Address(0xDEADBEEF);
        }
    };

    //        Undefined,           // Catch illegal address mode
    class AddressModeUndefined : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeUndefined(CPU &cpu) : AddressMode(cpu, "Undefined", "error") {}

        // Methods
    public:
        virtual hardware::Address execute()
        {
            std::cout << "AddressMode(Undefined): Not yet implemented" << std::endl;
            return hardware::Address(0xFADEFACE);
        }
    };

    //        IMPLICIT,           // Implicit
    class CPU::AddressModeImplied : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeImplied(CPU &cpu) : AddressMode(cpu, "Implicit", "") {};
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            std::cout << "AddressMode(Implied): Not yet implemented" << std::endl;
            return AddressMode::execute();
        };
    };

    //        ACCUMULATOR,        // Accumulator         A
    class CPU::AddressModeAccumulator : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeAccumulator(CPU &cpu) : AddressMode(cpu, "Accumulator", "A") {}
        // Methods
    public:
        virtual hardware::Address execute()
        {
            std::cout << "AddressModeAccumulator: Not yet implemented" << std::endl;
            return AddressMode::execute();
        }
    };

    //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    class CPU::AddressModeZeroPage : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPage(CPU &cpu) : AddressMode(cpu, "ZeroPage", "$nn") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);

            cpu.decodePipeline().operand = cpu.addressSpace.read(0x0000 + zpOffset);

            return zpOffset;
        }
    };

    //        ZERO_PAGE_X,        // Zero Page, X        $nn,X      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedX : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPageIndexedX(CPU &cpu) : AddressMode(cpu, "ZeroPage,X", "$nn,X") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
            cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.registers.X);

            return zpOffset + cpu.registers.X;
        }
    };

    //        ZERO_PAGE_Y,        // Zero Page, Y        $nn,Y      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedY : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPageIndexedY(CPU &cpu) : AddressMode(cpu, "ZeroPage, Y", "$nn,Y") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
            cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.registers.Y);

            return zpOffset + cpu.registers.Y;
        }
    };

    //        RELATIVE,           // Relative            $nn
    class CPU::AddressModeRelative : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeRelative(CPU &cpu) : AddressMode(cpu, "Relative", "$nn") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            std::cout << "AddressMode(Relative): Not yet implemented" << std::endl;
            return AddressMode::execute();
        }
    };

    //        ABSOLUTE,           // Absolute            $nnnn
    class CPU::AddressModeAbsolute : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeAbsolute(CPU &cpu) : AddressMode(cpu, "Absolute", "$nnnn") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
            cpu.decodePipeline().operand = cpu.addressSpace.read(absolute);
            cpu.registers.PC += 2;

            return absolute;
        }
    };

    //        ABSOLUTE_X,         // Absolute, X         $nnnn,X
    class CPU::AddressModeAbsoluteIndexedX : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeAbsoluteIndexedX(CPU &cpu) : AddressMode(cpu, "Absolute,X", "$nnnn,X") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            // value = read( $nnnn + X)
            AddressMode::execute();
            hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
            cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.registers.X);
            cpu.registers.PC += 2;

            return absolute + cpu.registers.X;
        }
    };

    //        ABSOLUTE_Y,         // Absolute, Y         $nnnn,Y
    class CPU::AddressModeAbsoluteIndexedY : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeAbsoluteIndexedY(CPU &cpu) : AddressMode(cpu, "Absolute,Y", "$nnnn,Y") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            // value = read( $nnnn + Y)
            AddressMode::execute();
            hardware::Address absolute = cpu.addressSpace.readWord(cpu.registers.PC);
            cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.registers.Y);
            cpu.registers.PC += 2;

            return absolute + cpu.registers.Y;
        }
    };

    //        INDIRECT,           // Indirect            ($nnnn)
    class CPU::AddressModeIndirect : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeIndirect(CPU &cpu) : AddressMode(cpu, "Indirect", "($nnnn)") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            hardware::Address indirectAddress = cpu.addressSpace.readWord(cpu.registers.PC++);

            cpu.decodePipeline().operand = cpu.addressSpace.readWord(indirectAddress);

            return indirectAddress;
        }
    };

    //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn,X)    LO bit 1
    class CPU::AddressModeIndexedIndirectX : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeIndexedIndirectX(CPU &cpu) : AddressMode(cpu, "X Indexed Indirect", "($nn,X)") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            // value = read( $nn + X )
            AddressMode::execute();
            hardware::Address zeroPageAddress = cpu.addressSpace.read(cpu.registers.PC++);
            hardware::Address address = cpu.addressSpace.readWord(zeroPageAddress + cpu.registers.X);
            cpu.decodePipeline().operand = cpu.addressSpace.read(address);

            return address;
        }
    };

    //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn),Y    LO bit 1
    class CPU::AddressModeIndirectIndexedY : public CPU::AddressMode
    {
        // Constructors
    public:
        AddressModeIndirectIndexedY(CPU &cpu) : AddressMode(cpu, "Y Indirect Indexed", "($nn),Y") {}
        // Methods
    public:
        virtual hardware::Address execute() override
        {
            AddressMode::execute();
            // read( read( $nn ) | (read( $nn + 1) << 8) + Y )
            // indirect pointer (PC | (PC+1)<<8) + Y
            // value = read( $nn + X )
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.registers.PC++);
            hardware::Address address = cpu.addressSpace.readWord(zpOffset);
            address += cpu.registers.Y;
            cpu.decodePipeline().operand = cpu.addressSpace.read(address);

            return address;
        }
    };

    //         IMMEDIATE,          // Immediate           #$nn
    class CPU::AddressModeImmediate : public CPU::AddressMode
    {
        // Types
    private:
    protected:
    public:
        // Constants
    private:
    protected:
    public:
        // Constructors
    private:
    protected:
    public:
        AddressModeImmediate(CPU &cpu) : AddressMode(cpu, "Immediate", "$#BB") {}
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual hardware::Address execute()
        {
            AddressMode::execute();

            cpu.decodePipeline().operand = cpu.addressSpace.read(cpu.registers.PC++);

            return cpu.addressSpace.read(cpu.registers.PC - 1); // TODO watch the PC value when opeand fetch is removed from this method
        }
        // Operators
    private:
    protected:
    public:
    };
}

#endif // ifndef ADDRESSMODE_HPP
