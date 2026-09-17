

#ifndef ADDRESSMODE_HPP
#define ADDRESSMODE_HPP

// #include <iostream>
// #include <iomanip>

#include "6502.hpp"

namespace m6502
{
    class CPU;

    // Addressing Modes

    // Instruction code chart
    // https://www.masswerk.at/6502/6502_instruction_set.html
    // enum class AddressModeKind
    // {
    //     IMPLICIT,           // Implicit
    //     ACCUMULATOR,        // Accumulator         A
    //     IMMEDIATE,          // Immediate           #$nn
    //     ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    //     ZERO_PAGE_X,        // Zero Page, X        $nn, X     LO bits 4,5,6
    //     ZERO_PAGE_Y,        // Zero Page, Y        $nn, Y     LO bits 4,5,6
    //     RELATIVE,           // Relative            $nnnn
    //     ABSOLUTE,           // Absolute            $nnnn
    //     ABSOLUTE_X,         // Absolute, X         $nnnn, X
    //     ABSOLUTE_Y,         // Absolute, Y         $nnnn, Y
    //     INDIRECT,           // Indirect            ($nnnn)
    //     INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn, X)   LO bit 1
    //     INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn), Y   LO bit 1
    // };

    class CPU::AddressMode // TODO should be abstract
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
        // TODO  this should return an address, a literal byte.... (flag bit, register(A,X,Y,PC,SP))
        virtual hardware::Address execute();
    };

    //        Undefined,           // Catch illegal address mode
    class CPU::AddressModeUndefined : public AddressMode
    {
        // Constructors
    public:
        AddressModeUndefined(CPU &cpu) : AddressMode(cpu, "Undefined", "error") {}

        // Methods
    public:
        hardware::Address execute() override;
    };

    //        IMPLICIT,           // Implicit
    class CPU::AddressModeImplied : public AddressMode
    {
        // Constructors
    public:
        AddressModeImplied(CPU &cpu) : AddressMode(cpu, "Implied", "") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        ACCUMULATOR,        // Accumulator         A
    class CPU::AddressModeAccumulator : public AddressMode
    {
        // Constructors
    public:
        AddressModeAccumulator(CPU &cpu) : AddressMode(cpu, "Accumulator", "A") {}
        // Methods
    public:
        virtual hardware::Address execute()
        {
            // TODO return 'Accumulator'
            std::cout << "AddressModeAccumulator: Not yet implemented" << std::endl;
            return AddressMode::execute();
        }
    };

    //        ZERO_PAGE,          // Zero Page           $nn        LO bits 4,5,6
    class CPU::AddressModeZeroPage : public AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPage(CPU &cpu) : AddressMode(cpu, "ZeroPage", "$nn") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        ZERO_PAGE_X,        // Zero Page, X        $nn,X      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedX : public AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPageIndexedX(CPU &cpu) : AddressMode(cpu, "ZeroPage,X", "$nn,X") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        ZERO_PAGE_Y,        // Zero Page, Y        $nn,Y      LO bits 4,5,6
    class CPU::AddressModeZeroPageIndexedY : public AddressMode
    {
        // Constructors
    public:
        AddressModeZeroPageIndexedY(CPU &cpu) : AddressMode(cpu, "ZeroPage, Y", "$nn,Y") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        RELATIVE,           // Relative            $nn
    class CPU::AddressModeRelative : public AddressMode
    {
        // Constructors
    public:
        AddressModeRelative(CPU &cpu) : AddressMode(cpu, "Relative", "$nn") {}
        // Methods
    public:
        virtual hardware::Address execute() override;
    };

    //        ABSOLUTE,           // Absolute            $nnnn
    class CPU::AddressModeAbsolute : public AddressMode
    {
        // Constructors
    public:
        AddressModeAbsolute(CPU &cpu) : AddressMode(cpu, "Absolute", "$nnnn") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        ABSOLUTE_X,         // Absolute, X         $nnnn,X
    class CPU::AddressModeAbsoluteIndexedX : public AddressMode
    {
        // Constructors
    public:
        AddressModeAbsoluteIndexedX(CPU &cpu) : AddressMode(cpu, "Absolute,X", "$nnnn,X") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        ABSOLUTE_Y,         // Absolute, Y         $nnnn,Y
    class CPU::AddressModeAbsoluteIndexedY : public AddressMode
    {
        // Constructors
    public:
        AddressModeAbsoluteIndexedY(CPU &cpu) : AddressMode(cpu, "Absolute,Y", "$nnnn,Y") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        INDIRECT,           // Indirect            ($nnnn)
    class CPU::AddressModeIndirect : public AddressMode
    {
        // Constructors
    public:
        AddressModeIndirect(CPU &cpu) : AddressMode(cpu, "Indirect", "($nnnn)") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        INDEXED_INDIRECT_X, // X Indexed Indirect  ($nn,X)    LO bit 1
    class CPU::AddressModeIndexedIndirectX : public AddressMode
    {
        // Constructors
    public:
        AddressModeIndexedIndirectX(CPU &cpu) : AddressMode(cpu, "X Indexed Indirect", "($nn,X)") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //        INDIRECT_INDEXED_Y  // Y Indirect Indexed  ($nn),Y    LO bit 1
    class CPU::AddressModeIndirectIndexedY : public AddressMode
    {
        // Constructors
    public:
        AddressModeIndirectIndexedY(CPU &cpu) : AddressMode(cpu, "Y Indirect Indexed", "($nn),Y") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    //         IMMEDIATE,          // Immediate           #$nn
    class CPU::AddressModeImmediate : public AddressMode
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
        hardware::Address execute() override;
        // Operators
    private:
    protected:
    public:
    };

    //        Stack (pseudo),      // Stack               $01nn
    class CPU::AddressModeStack : public AddressMode // TODO should be abstract
    {
    public:
        const inline static hardware::Byte stackPage = 0x01;
        // const inline static hardware::Address stackMask = 0x01FF;

        // Constructors
    public:
        AddressModeStack(CPU &cpu, const char *name, const char *mnemonic) : AddressMode(cpu, name, mnemonic) {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    class CPU::AddressModeStackPull : public AddressModeStack
    {
        // Constructors
    public:
        AddressModeStackPull(CPU &cpu) : AddressModeStack(cpu, "Stack Pull", "$01nn") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

    class CPU::AddressModeStackPush : public AddressModeStack
    {
        // Constructors
    public:
        AddressModeStackPush(CPU &cpu) : AddressModeStack(cpu, "Stack Push", "$01nn") {}
        // Methods
    public:
        hardware::Address execute() override;
    };

}

#endif