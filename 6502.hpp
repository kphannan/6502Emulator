

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
// #include <cstddef>

#include "hardware.hpp"
#include "memory.hpp"

namespace m6502
{

    typedef struct GeneralPurposeRegisters
    {
        hardware::Byte A; // Accumulator

        hardware::Byte Y; // Index register Y
        hardware::Byte X; // Index register  X

        hardware::Byte S; // Stack Pointer

        hardware::Address PC; // Program Counter
    } REGISTERS;

    // Bit definitions of the Processor Status Register (P)
    typedef struct ProcessorStatusRegister
    {
        int N : 1;   // Negative
        int V : 1;   // Overflow
        int one : 1; // Constant '1'
        int B : 1;   // BRK command  1 = BRK, 0 = IRQB
        int D : 1;   // Decimal Mode 1 = true
        int I : 1;   // IRQB disable 1 = disable
        int Z : 1;   // Zero 1 = true
        int C : 1;   // Carry 1 = true
    } STATUS_FLAGS;

    typedef struct ProgrammingModel
    {
        // General Purpose  Registers
        REGISTERS registers;

        // Processor Status Register, accessible as a byte or indiviidual bits.
        union
        {
            hardware::Byte P;
            STATUS_FLAGS flags;
        };
    } PROGRAMMING_MODEL;

    // Default memory spans the whole address space
    // MEMORY *memory;

    // Addressing Modes
    // Implicit
    // Accumulator         A
    // Immediate           #$nn
    // Zero Page           $nn        LO bits 4,5,6
    // Zero Page, X        $nn, X     LO bits 4,5,6
    // Zero Page, Y        $nn, Y     LO bits 4,5,6
    // Relative            $nnnn
    // Absolute            $nnnn
    // Absolute, X         $nnnn, X
    // Absolute, Y         $nnnn, Y
    // Indirect            ($nnnn)
    // X Indexed Indirect  ($nn, X)   LO bit 1
    // Y Indirect Indexed  ($nn), Y   LO bit 1

    // Hardware / Software Vectors
    enum class HardwareVector
    {
        NMI = 0xFFFA,
        RESET = 0xFFFC,
        IRQ = 0xFFFE
    };

    /**
     * @brief A model of the registers (programming model) of the 6502 microprocessor.
     *
     */
    class CPU
    {
        // ----- Constants -----
    private:
    protected:
    public:
        // ----- Attributes -----
    private:
        PROGRAMMING_MODEL model;
        memory::Memory &memory; // make a reference

        hardware::Byte instruction;

    protected:
    public:
        // ----- Constructors -----
    private:
    protected:
    public:
        CPU();
        CPU(memory::Memory &memory);

        // ----- Methods -----
    private:
    protected:
        void fetchOpCode();
        void fetchOperand(); // this considers addressing mode

    public:
        void reset();
        // void memoryBank(Memory *memory); // currently only support full address space.
        void showRegisters();

        void execute(hardware::Address address);
        void execute(int numberOfInstructions);
        void execute();

        memory::Memory currentMemory();
    };

}

#endif
