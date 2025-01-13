#ifndef PIPELINE_HPP
#define PIPELINE_HPP

// #include "6502.hpp"
// #include "hardware.hpp"
#include "memory.hpp"

namespace m6502
{

    // union Instruction
    // {
    //     hardware::Byte opcode;
    //     struct foo
    //     {
    //         unsigned c : 2; // 1..0
    //         unsigned b : 3; // 4..2
    //         unsigned a : 3; // 7..5
    //         int z : 1;
    //     } bits;
    // };

    // // Addressing Modes

    // // Instruction code chart
    // // https://www.masswerk.at/6502/6502_instruction_set.html
    // enum class AddressMode
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

    // // Instruction Pipeline
    // // fetch OpCode (advance PC)
    // // decode OpCode
    // // fetch operand (advance PC as defined by operand)
    // // evaluate OpCode (set status flags)

    // class Pipeline
    // {
    //     // Constants
    // private:
    // protected:
    // public:
    //     // Fields
    // private:
    //     CPU &cpu;
    //     // The Address space connected to the CPU
    //     memory::Memory &addressSpace;
    //     hardware::Address &address; // Same as program counter

    //     Instruction instruction;
    //     // int opcode;
    //     int operand; // struct/union/class (register,implied,Byte,Word)
    //     AddressMode addressMode;

    // protected:
    // public:
    //     // Constructors
    // private:
    // protected:
    // public:
    //     /** Configure the instruction pipeline with the address space and program counter. */
    //     // Pipeline(memory::Memory &memory, hardware::Address &address);
    //     Pipeline(CPU &cpu);
    //     // Pipeline(memory::Memory &memory);

    //     // ~Pipeline();

    //     // Methods
    // private:
    //     // Read an opcode from the cuurrent memory address
    //     void fetchOpCode();
    //     AddressMode decodeAddressMode(const Instruction instruction);
    //     void showAddressMode(const AddressMode addressMode) const;
    //     void decodeSource();
    //     void decodeDestination();
    //     void decodeOperation();

    //     // Decode  the opcode and determine the addressing mode and read the operand
    //     void fetchOperand(const AddressMode addressMode);

    // protected:
    // public:
    //     void reset(hardware::Address address);
    //     // Reset the pipeline, removing any instructions being decoded.
    //     void clear();
    //     void execute(int numberOfSteps);
    //     void showPipeline() const;
    //     //  Registers

    //     // Operators
    // private:
    // protected:
    // public:
    // };
}

#endif // #ifndef PIPELINE_HPP