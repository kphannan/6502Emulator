

#ifndef INSTRUCTIONSET_HPP
#define INSTRUCTIONSET_HPP

class InstructionSet
{
private:
protected:
public:
    // Addressing Modes

    // Instruction code chart
    // https://www.masswerk.at/6502/6502_instruction_set.html
    enum class AddressModeKind
    {
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
    };

    // Constants
private:
protected:
public:
    // Constructors
private:
protected:
public:
    // Fields
private:
protected:
public:
    // Methods
private:
protected:
public:
    // Operators
private:
protected:
public:
}

#endif // ifndef INSTRUCTIONSET_HPP