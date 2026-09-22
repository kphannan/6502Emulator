

#include <gtest/gtest.h>

#include "6502.hpp"
#include "AddressMode.hpp"
#include "memory.hpp"

namespace m6502 {

    // Address Mode classes execute() method will return the address
    // the processor will act on, not the value of the program counter.
    class AddressModeTest : public testing::Test {
        public:
            // std::unique_ptr<memory::Memory> memory = std::make_unique<memory::Memory>( "UnitTest - AddressMode" );
            memory::Memory memory = *(new memory::Memory( "UnitTest - AddressMode" ));

            CPU* cpu;

        protected:
        AddressModeTest()
        {
            memory[0x00FC] = 0xDE;
            memory[0x00FD] = 0xFA;
            // Destination of the reset vector - leaves zeroPage available for
            // testing
            memory[0x2000] = 0x49;  // LDA #00 // starting instruction after reset
            memory[0x2001] = 0x5A;  //

            // Reset vector points to start of memory
            memory[0xFFFC] = 0x00;  // cpu::HardwareVector::RESET
            memory[0xFFFD] = 0x20;  //      MSB

            cpu = new CPU(memory);
        }

        ~AddressModeTest() override { delete cpu; }
    };

    // Addressing Modes
    // ----- Immediate #$BB
    TEST_F(AddressModeTest, Immediate)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeImmediate>(*cpu);

        memory[0x3215] = 0x97;
        memory[0x3216] = 0x98;
        memory[0x3217] = 0x99;

        cpu->PC(0x3216);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3217, cpu->PC());
        EXPECT_EQ(0x3216, address);
        EXPECT_EQ(0x98, memory[address]);
    }

    // ..... Implied
    // ..... Accumulator

    // ----- ZeroPage $LL
    TEST_F(AddressModeTest, ZeroPage)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeZeroPage>(*cpu);

        memory[0x0064] = 0x76; // data
        memory[0x0065] = 0xDD; // data

        memory[0x3210] = 0x64; // Address Mode
        memory[0x3211] = 0x65; // Address Mode

        cpu->PC(0x3210);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0064, address);
        EXPECT_EQ(0x76, memory[address]);
    }

    // ----- ZeroPage,X $LL,X
    TEST_F(AddressModeTest, ZeroPageX)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeZeroPageIndexedX>(*cpu);
        memory[0x0064] = 0x44; // data
        memory[0x3210] = 0x60; // Address Mode

        cpu->PC(0x3210);
        cpu->X(0x04);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0064, address);
        EXPECT_EQ(0x44, memory[address]);
    }

    // ----- ZeroPage,Y $LL,Y
    TEST_F(AddressModeTest, ZeroPageY)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeZeroPageIndexedY>(*cpu);
        memory[0x0074] = 0x66; // data
        memory[0x3210] = 0x64; // Address Mode

        cpu->PC(0x3210);
        cpu->Y(0x10);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3211, cpu->PC());
        EXPECT_EQ(0x0074, address);
        EXPECT_EQ(0x66, memory[address]);
    }

    // ----- Relative $BB
    TEST_F(AddressModeTest, Relative_Forward_Max)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeRelative>(*cpu);
        memory[0x6000] = 0x7F; // +127
        cpu->PC(0x6000);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x6001, cpu->PC());
        EXPECT_EQ(0x6080, address);
    }

    TEST_F(AddressModeTest, Relative_Forward_Min)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeRelative>(*cpu);
        memory[0x6000] = 0x00; // +0
        cpu->PC(0x6000);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x6001, cpu->PC());
        EXPECT_EQ(0x6001, address);
    }

    TEST_F(AddressModeTest, Relative_Forward_One)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeRelative>(*cpu);
        memory[0x6000] = 0x01; // +1
        cpu->PC(0x6000);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x6001, cpu->PC());
        EXPECT_EQ(0x6002, address);
    }




    TEST_F(AddressModeTest, Relative_Reverse_Max)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeRelative>(*cpu);
        memory[0x6000] = 0x80; // -128
        cpu->PC(0x6000);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x6001, cpu->PC());
        EXPECT_EQ(0x5F81, address);
    }

    TEST_F(AddressModeTest, Relative_Reverse_Min)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeRelative>(*cpu);
        memory[0x6000] = 0xFF; // -1
        cpu->PC(0x6000);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x6001, cpu->PC());
        EXPECT_EQ(0x6000, address);
    }




    // ----- Absolute $LLHH
    TEST_F(AddressModeTest, Absolute)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeAbsolute>(*cpu);
        memory[0x8864] = 0x99; // data
        memory[0x3210] = 0x64; // Address Mode
        memory[0x3211] = 0x88; // Address Mode
        cpu->PC(0x3210);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8864, address);
        EXPECT_EQ(0x99, memory[address]);
    }

    // ----- AbsoluteX $LLHH,X
    TEST_F(AddressModeTest, AbsoluteX)
    {
        // --- given
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeAbsoluteIndexedX>(*cpu);
        memory[0x8864] = 0xAD; // data
        memory[0x8865] = 0xDE; // data
        memory[0x8874] = 0x60; // data
        memory[0x8875] = 0x05; // data

        memory[0x3210] = 0x64; // pcl
        memory[0x3211] = 0x88; // pch

        cpu->PC(0x3210);
        cpu->X(0x10);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8874, address);
        EXPECT_EQ(0x60, memory[address]);
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
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeAbsoluteIndexedY>(*cpu);
        memory[0x8864] = 0x99; // data
        memory[0x8874] = 0x05; // data
        memory[0x8884] = 0x50; // data

        memory[0x3210] = 0x64; // Address Mode pcl
        memory[0x3211] = 0x88; // Address Mode pch

        cpu->PC(0x3210);
        cpu->Y(0x20);

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x3212, cpu->PC());
        EXPECT_EQ(0x8884, address);
        EXPECT_EQ(0x50, memory[address]);
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
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeIndexedIndirectX>(*cpu);

        cpu->X(0x10);
        cpu->PC(0x2003);

        // memory.write(0x2000, 0xA2); // LDX #$05
        // memory.write(0x2001, 0x05); // Base of lookup table in page zero
        // memory.write(0x2002, 0xA1); // LDA ($70,X)
        memory[0x2003] = 0x70; // offset from base address

        // Lookup table of addresses
        memory[0x0070] = 0x11; // Entry $00, $LL Address lookup table
        memory[0x0071] = 0x11; //            $HH
        memory[0x0072] = 0x22; // Entry $00, $LL Address lookup table
        memory[0x0073] = 0x22; //            $HH
        // .. ... ... ... ...
        memory[0x0080] = 0x23; // Entry $10, $LL Address lookup table
        memory[0x0081] = 0x30; //            $HH

        // Data pointed to from the table.
        memory[0x1111] = 0x20; // Data
        memory[0x2222] = 0x82; // Data
        memory[0x3023] = 0xA5; // Data

        // --- when
        hardware::Address address = mode->execute();
        hardware::Byte    value   = memory[address];

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
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeIndirectIndexedY>(*cpu);
        cpu->Y(0x10);
        cpu->PC(0x2003);

        memory[0x2003] = 0x70; // offset from base address

        // Address of lookup table
        memory[0x0070] = 0x34; // Table entry $01 $LL
        memory[0x0071] = 0x12; //                 $HH

        // base of the address table
        memory[0x1234] = 0x00; // Table entry $01 $LL
        memory[0x1235] = 0x82; //                 $HH
        memory[0x1236] = 0x10; // Table entry $02 $LL
        memory[0x1237] = 0x82; //                 $HH
        //  .. .....   ......          .....        .....
        memory[0x1244] = 0x60; // Table entry $10 $LL
        memory[0x1245] = 0x82; //                 $HH

        memory[0x8200] = 0x11;
        memory[0x8210] = 0x22;
        //  .. .....   ......          .....        .....
        memory[0x8260] = 0x49;

        // --- when
        hardware::Address address = mode->execute();

        hardware::Byte value = memory[address];

        // --- then
        EXPECT_EQ(0x2004, cpu->PC());
        EXPECT_EQ(0x10, cpu->Y());
        EXPECT_EQ(0x8260, address);
        EXPECT_EQ(0x49, value);
    }

    // push value then move stack pointer (-)
    TEST_F(AddressModeTest, StackPush)
    {
        // --- given
        cpu->S(0xF2);
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeStackPush>(*cpu);

        memory[0x01F2] = 0x99;
        memory[0x01F1] = 0xAA;

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x01F1, cpu->S());
        EXPECT_EQ(0x01F2, address);
        EXPECT_EQ(0x99, memory[address]);
    }

    // Move stack pointer (+) then read value
    TEST_F(AddressModeTest, StackPull)
    {
        // --- given
        cpu->S(0xF2);
        std::unique_ptr<CPU::AddressMode> mode = std::make_unique<CPU::AddressModeStackPull>(*cpu);

        memory[0x01F1] = 0xAA;
        memory[0x01F2] = 0x99;
        memory[0x01F3] = 0x11;

        // --- when
        hardware::Address address = mode->execute();

        // --- then
        EXPECT_EQ(0x01F3, cpu->S());
        EXPECT_EQ(0x01F3, address);
        EXPECT_EQ(0x11, memory[address]);
    }


}  // namespace m6502
