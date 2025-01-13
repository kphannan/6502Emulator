

#ifndef ADDRESSMODE_HPP
#define ADDRESSMODE_HPP

#include <iostream>
#include <iomanip>

#include "6502.hpp"

namespace m6502
{
    class CPU;

    class CPU::AddressMode
    {
        // Types
    private:
    protected:
    public:
        // Addressing Modes

        // // Instruction code chart
        // // https://www.masswerk.at/6502/6502_instruction_set.html
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

        // Constants
    private:
    protected:
    public:
        // Constructors
    private:
    protected:
        AddressMode(const char *name, const char *mnemonic) : modeName(name), modeMnemonic(mnemonic) {};

    public:
        // Fields
    private:
        const char *modeName;
        const char *modeMnemonic;

    protected:
    public:
        // Methods
    private:
    protected:
    public:
        const char *name() const { return modeName; };
        const char *mnemonic() const { return modeMnemonic; };
        virtual void execute() {};
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
        AddressModeUndefined() : AddressMode("Undefined", "error") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
        // Operators
    private:
    protected:
    public:
    };

    //        IMPLICIT,           // Implicit
    class CPU::AddressModeImplicit : public CPU::AddressMode
    {
        // Constructors
    private:
    protected:
    public:
        AddressModeImplicit() : AddressMode("Immediate", "$#BB") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeAccumulator() : AddressMode("Accumulator", "A") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeZeroPage() : AddressMode("ZeroPage", "$nn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeZeroPageIndexedX() : AddressMode("ZeroPage,X", "$nn,X") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeZeroPageIndexedY() : AddressMode("ZeroPage, Y", "$nn,Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeRelative() : AddressMode("Relative", "$nnnn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeAbsolute() : AddressMode("Absolute", "$nnnn") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeAbsoluteIndexedX() : AddressMode("Absolute,X", "$nnnn,X") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeAbsoluteIndexedY() : AddressMode("Absolute,Y", "$nnnn,Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeIndirect() : AddressMode("Indirect", "($nnnn)") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeIndexedIndirectX() : AddressMode("X Indexed Indirect", "($nn, X)") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeIndirectIndexedY() : AddressMode("Y Indirect Indexed", "($nn),Y") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << "Not yet implemented" << std::endl; };
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
        AddressModeImmediate() : AddressMode("Immediate", "$#BB") {};
        // Fields
    private:
    protected:
    public:
        // Methods
    private:
    protected:
    public:
        virtual void execute() { std::cout << name() << " ; " << mnemonic() << std::endl; /* operand = cpu.addressSpace.read(cpu.model.registers.PC++); */ };
        // Operators
    private:
    protected:
    public:
    };
}
// std::cout << "#       OPC #$BB";
// operand = cpu.addressSpace.read(cpu.model.registers.PC++);

#endif // ifndef ADDRESSMODE_HPP