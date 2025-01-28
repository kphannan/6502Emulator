

#include <gtest/gtest.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class AddressModeTest : public testing::Test
    {
    public:
        memory::Memory *tMemory = new memory::Memory("AddrModeUnitTestMemory");
        memory::Memory memory = *tMemory;

        CPU *cpu;

    protected:
        AddressModeTest()
        {
            // Destination of the reset vector - leaves zeroPage available for testing
            memory.write(0x2000, 0x49); // LDA #00 // starting instruction after reset
            memory.write(0x2001, 0x5A); //

            // Reset vector points to start of memory
            memory.write(0xFFFC, 0x00); // cpu::HardwareVector::RESET
            memory.write(0xFFFD, 0x20); //      MSB

            cpu = new CPU(memory);
        }

        ~AddressModeTest() override
        {
            delete cpu;
        }
    };

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(AddressModeTest, Immediate)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeImmediate(*cpu);
        memory.write(0x3216, 0x98);
        cpu->PC(0x3216);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3217, cpu->PC());
        EXPECT_EQ(0x3216, address);
        EXPECT_EQ(0x98, memory.read(address));
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(AddressModeTest, ZeroPage)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeZeroPage(*cpu);
        memory.write(0x0064, 0x76); // data
        memory.write(0x3210, 0x64); // Address Mode
        cpu->PC(0x3210);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0064, address);
        EXPECT_EQ(0x76, memory.read(address));
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(AddressModeTest, ZeroPageX)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeZeroPageIndexedX(*cpu);
        memory.write(0x0064, 0x44); // data
        memory.write(0x3210, 0x60); // Address Mode
        cpu->PC(0x3210);
        cpu->X(0x04);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0064, address);
        EXPECT_EQ(0x44, memory.read(address));
    }

    // ----- ZeroPage,Y $LL,Y
    TEST_F(AddressModeTest, ZeroPageY)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeZeroPageIndexedY(*cpu);
        memory.write(0x0074, 0x66); // data
        memory.write(0x3210, 0x64); // Address Mode
        cpu->PC(0x3210);
        cpu->Y(0x10);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0074, address);
        EXPECT_EQ(0x66, memory.read(address));
    }

    // ----- Relative $BB

    // ----- Absolute $LLHH
    TEST_F(AddressModeTest, Absolute)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeAbsolute(*cpu);
        memory.write(0x8864, 0x99); // data
        memory.write(0x3210, 0x64); // Address Mode
        memory.write(0x3211, 0x88); // Address Mode
        cpu->PC(0x3210);
        // cpu->Y(0x10);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8864, address);
        EXPECT_EQ(0x99, memory.read(address));
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(AddressModeTest, AbsoluteX)
    {
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeAbsoluteIndexedX(*cpu);
        memory.write(0x8874, 0x60); // data
        memory.write(0x8875, 0x05); // data

        memory.write(0x3210, 0x64); // Address Mode
        memory.write(0x3211, 0x88); // Address Mode
        cpu->PC(0x3210);
        cpu->X(0x10);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8874, address);
        EXPECT_EQ(0x60, memory.read(address));
    }

    // ----- AbsoluteY $LLHH,Y
    TEST_F(AddressModeTest, AbsoluteY)
    {
        //  Y: $20
        // PC: $3210
        // $3210/1: $8864
        //       Y:   $20
        //          $8884
        // $8884: $50  - the data
        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeAbsoluteIndexedY(*cpu);
        memory.write(0x8864, 0x99); // data
        memory.write(0x8874, 0x05); // data
        memory.write(0x8884, 0x50); // data

        memory.write(0x3210, 0x64); // Address Mode
        memory.write(0x3211, 0x88); // Address Mode
        cpu->PC(0x3210);
        cpu->Y(0x20);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8884, address);
        EXPECT_EQ(0x50, memory.read(address));
    }

    // ----- Indirect ($LLHH)
    // TEST_F(AddressModeTest, Indirect)
    // {
    //     ADD_FAILURE_AT(__FILE__, __LINE__);
    // }

    // ----- Indexed Indirect X ($LL,X)
    TEST_F(AddressModeTest, IndexedIndirectX)
    {
        // $nn: $70
        //   X: $10
        //        ┌───────┐
        // $0070  │ $1111 │           1st table entry
        //        └───────┘
        //        ┌───────┐
        // $0072  │ $2222 │           2nd table entry
        //   :    └───────┘
        //   :
        //   :    ┌───────┐
        // $0080  │ $3023 │ ───>──┐   X-th entry in table of addresses
        //        └───────┘       │
        //                        │
        //        ┌─────┐         │
        // $1111  │ $20 │         │
        //        └─────┘         │
        //        ┌─────┐         │
        // $2222  │ $82 │         │
        //        └─────┘         │
        //   ┌──<─────────────<───┘
        //   │    ┌─────┐
        // $7777  │ $49 │
        //        └─────┘

        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeIndexedIndirectX(*cpu);
        cpu->X(0x10);
        cpu->PC(0x2003);
        // memory.write(0x2000, 0xA2); // LDX #$05
        // memory.write(0x2001, 0x05); // Base of lookup table in page zero
        // memory.write(0x2002, 0xA1); // LDA ($70,X)
        memory.write(0x2003, 0x70); // offset from base address

        // Lookup table of addresses
        memory.write(0x0070, 0x11); // Entry $00, $LL Address lookup table
        memory.write(0x0071, 0x11); //            $HH
        memory.write(0x0072, 0x22); // Entry $00, $LL Address lookup table
        memory.write(0x0073, 0x22); //            $HH
        // .. ... ... ... ...
        memory.write(0x0080, 0x23); // Entry $10, $LL Address lookup table
        memory.write(0x0081, 0x30); //            $HH

        // Data pointed to from the table.
        memory.write(0x1111, 0x20); // Data
        memory.write(0x2222, 0x82); // Data
        memory.write(0x3023, 0xA5); // Data

        // --- when
        hardware::Address address = mode->execute();

        hardware::Byte value = memory.read(address);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x10, cpu->X());
        EXPECT_EQ(0x3023, address);
        EXPECT_EQ(0xA5, value);
    }

    // ----- Indirect Indexed Y ($LL),Y
    TEST_F(AddressModeTest, IndirectIndexedY)
    {
        // $nn: $70
        //   Y: $10
        //        ┌───────┐
        // $0070  │ $1234 │        $nn - points to a table of addresses ($1234)
        //        └───────┘         │
        //                          │
        //              ┌───────<───┘
        //        ┌───────┐
        // $1234  │ $8200 │             1st address table entry
        //        └───────┘
        //        ┌───────┐
        // $1236  │ $8210 │             2nd address table entry
        //   :    └───────┘
        //   :        :
        //   :        :
        //   :    ┌───────┐
        // $1244  │ $8260 │ ──>──┐     Y-th entry in table
        //        └───────┘      │
        //             ┌─<────<──┘
        //        ┌─────┐
        // $8260  │ $49 │
        //        └─────┘

        // --- given
        CPU::AddressMode *mode = new CPU::AddressModeIndirectIndexedY(*cpu);
        cpu->Y(0x10);
        cpu->PC(0x2003);

        memory.write(0x2003, 0x70); // offset from base address

        // Address of lookup table
        memory.write(0x0070, 0x34); // Table entry $01 $LL
        memory.write(0x0071, 0x12); //                 $HH

        // base of the address table
        memory.write(0x1234, 0x00); // Table entry $01 $LL
        memory.write(0x1235, 0x82); //                 $HH
        memory.write(0x1236, 0x10); // Table entry $02 $LL
        memory.write(0x1237, 0x82); //                 $HH
        //  .. .....   ......          .....        .....
        memory.write(0x1244, 0x60); // Table entry $10 $LL
        memory.write(0x1245, 0x82); //                 $HH

        memory.write(0x8200, 0x11);
        memory.write(0x8210, 0x22);
        //  .. .....   ......          .....        .....
        memory.write(0x8260, 0x49);

        // --- when
        hardware::Address address = mode->execute();

        hardware::Byte value = memory.read(address);

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0x8260, address);
        EXPECT_EQ(0x49, value);
    }

    TEST_F(AddressModeTest, StackPush)
    {
        // --- given
        cpu->S(0xF2);
        CPU::AddressMode *mode = new CPU::AddressModeStackPush(*cpu);

        memory.write(0x01F2, 0x99);
        memory.write(0x01F1, 0xAA);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0xF1, cpu->S());
        EXPECT_EQ(0x01F2, address);
        EXPECT_EQ(0x99, memory.read(address));
    }

    TEST_F(AddressModeTest, StackPull)
    {
        // --- given
        cpu->S(0xF2);
        CPU::AddressMode *mode = new CPU::AddressModeStackPull(*cpu);

        memory.write(0x01F3, 0x11);
        memory.write(0x01F2, 0x99);
        memory.write(0x01F1, 0xAA);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0xF3, cpu->S());
        EXPECT_EQ(0x01F3, address);
        EXPECT_EQ(0x11, memory.read(address));
    }

    // TEST_F(AddressModeTest, ZeroPage)
    // {
    //     memory::Memory &memory = cpu->currentMemory();

    //     // --- Make sure there is a full memory by default
    //     EXPECT_STREQ("UnitTestMemory", memory.name());
    //     EXPECT_EQ(0xFFFF, memory.memorySize());
    //     EXPECT_EQ(0xFFFF, memory.memorySize());
    //     EXPECT_EQ(0x0000, memory.lowAddress());
    //     EXPECT_EQ(0xFFFF, memory.highAddress());

    //     EXPECT_EQ(0x2000, cpu->PC());
    //     EXPECT_EQ(0x00, cpu->A());
    //     EXPECT_EQ(0x00, cpu->X());
    //     EXPECT_EQ(0x00, cpu->Y());
    //     // EXPECT_EQ(0x20, cpu->P());
    //     EXPECT_EQ(0b00100000, cpu->P());
    // }

}
