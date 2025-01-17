

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
        ACCUMULATOR,        // Accumulator         A
        IMMEDIATE,          // Immediate           #$nn
        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
        ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6
        ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6
        RELATIVE,           // Relative            $nnnn
        ABSOLUTE,           // Absolute            $nnnn
        ABSOLUTE_X,         // Absolute, X         $nnnn, X
        ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y
        INDIRECT,           // Indirect            ($nnnn)
        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
    */

    class CPU;

    class CPU::AddressMode
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
        AddressMode(CPU &cpu, const char *name, const char *mnemonic) : cpu(cpu), modeName(name), modeMnemonic(mnemonic) {};

    public:
        // Fields
    private:
        const char *modeName;
        const char *modeMnemonic;

    protected:
        CPU &cpu;

        // Methods
    private:
    protected:
    public:
        const char *name() const { return modeName; };
        const char *mnemonic() const { return modeMnemonic; };
        virtual void execute() { std::cout << name() << " ; " << mnemonic() << std::endl; };
        // Operators
    private:
    protected:
    public:
    };

    //        IMPLICIT,           // Implicit
    class AddressModeUndefined : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeUndefined(CPU &cpu) : AddressMode(cpu, "Undefined", "error") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "AddressMode(Undefined): Not yet implemented" << std::endl; };
        // Operators
    private:
    protected:
    public:
    };

    //        IMPLICIT,           // Implicit
    class CPU::AddressModeImplied : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeImplied(CPU &cpu) : AddressMode(cpu, "Implicit", "") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            std::cout << "AddressMode(Implied): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };

    //        ACCUMULATOR,        // Accumulator         A
    class CPU::AddressModeAccumulator : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeAccumulator(CPU &cpu) : AddressMode(cpu, "Accumulator", "A") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            std::cout << "AddressMode(Accumulator): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    class CPU::AddressModeZeroPage : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeZeroPage(CPU &cpu) : AddressMode(cpu, "ZeroPage", "$nn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.model.registers.PC++);
            cpu.decodePipeline().operand = cpu.addressSpace.read(0x0000 + zpOffset);
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedX : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeZeroPageIndexedX(CPU &cpu) : AddressMode(cpu, "ZeroPage,X", "$nn,X") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.model.registers.PC++);
            cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.model.registers.X);
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedY : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeZeroPageIndexedY(CPU &cpu) : AddressMode(cpu, "ZeroPage, Y", "$nn,Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            hardware::Address zpOffset = cpu.addressSpace.read(cpu.model.registers.PC++);
            cpu.decodePipeline().operand = cpu.addressSpace.read(zpOffset + cpu.model.registers.Y);
        };
        // Operators
    private:
    protected:
    public:
    };
    //        RELATIVE,           // Relative            $nnnn
    class CPU::AddressModeRelative : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeRelative(CPU &cpu) : AddressMode(cpu, "Relative", "$nnnn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            std::cout << "AddressMode(Relative): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ABSOLUTE,           // Absolute            $nnnn
    class CPU::AddressModeAbsolute : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeAbsolute(CPU &cpu) : AddressMode(cpu, "Absolute", "$nnnn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            // std::cout << "AddressMode(Absolute): Not yet implemented" << std::endl;
            hardware::Address absolute = cpu.addressSpace.readWord(cpu.model.registers.PC);
            cpu.decodePipeline().operand = cpu.addressSpace.read(absolute);
            cpu.model.registers.PC += 2;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ABSOLUTE_X,         // Absolute, X         $nnnn, X
    class CPU::AddressModeAbsoluteIndexedX : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeAbsoluteIndexedX(CPU &cpu) : AddressMode(cpu, "Absolute,X", "$nnnn,X") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            // value = read( $nnnn + X)
            AddressMode::execute();
            hardware::Address absolute = cpu.addressSpace.readWord(cpu.model.registers.PC);
            cpu.decodePipeline().operand = cpu.addressSpace.read(absolute + cpu.model.registers.X);
            cpu.model.registers.PC += 2;
//            std::cout << "AddressMode(AbsoluteX): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y
    class CPU::AddressModeAbsoluteIndexedY : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeAbsoluteIndexedY(CPU &cpu) : AddressMode(cpu, "Absolute,Y", "$nnnn,Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            // value = read( $nnnn + Y)
            AddressMode::execute();
            std::cout << "AddressMode(AbsoluteY): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        INDIRECT,           // Indirect            ($nnnn)
    class CPU::AddressModeIndirect : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeIndirect(CPU &cpu) : AddressMode(cpu, "Indirect", "($nnnn)") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            std::cout << "AddressMode(Indirect): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
    class CPU::AddressModeIndexedIndirectX : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeIndexedIndirectX(CPU &cpu) : AddressMode(cpu, "X Indexed Indirect", "($nn, X)") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            // value = read( $nn + X )
            AddressMode::execute();
            std::cout << "AddressMode(IndexedIndirectX): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
    };
    //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
    class CPU::AddressModeIndirectIndexedY : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeIndirectIndexedY(CPU &cpu) : AddressMode(cpu, "Y Indirect Indexed", "($nn),Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            // read( read( $nn ) | (read( $nn + 1) << 8) + Y )
            // indirect pointer (PC | (PC+1)<<8) + Y
            std::cout << "AddressMode(IndirectIIndexedY): Not yet implemented" << std::endl;
        };
        // Operators
    private:
    protected:
    public:
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
        AddressModeImmediate(CPU &cpu) : AddressMode(cpu, "Immediate", "$#BB") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute()
        {
            AddressMode::execute();
            cpu.decodePipeline().operand = cpu.addressSpace.read(cpu.model.registers.PC++);
        };
        // Operators
    private:
    protected:
    public:
    };
}

#endif // ifndef ADDRESSMODE_HPP
