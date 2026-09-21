

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "memory.hpp"

namespace memory
{
    class MemoryTest : public testing::Test
    {
    public:
        memory::Memory memory = *(new memory::Memory( "UnitTest - Memory" ));

    protected:
        MemoryTest() //: memory(*(new Memory("test suite")))
        {
            memory.fill(0xAA);
        }
    };

    // ===============================================================
    // === Non-Banked memory (lower bound is 0x0000)
    TEST_F(MemoryTest, ConstructDefaultAddressSpace)
    {
        // --- given
        // --- when
        Memory testBank;

        // --- then
        // EXPECT_TRUE(nullptrptr != memory);
        EXPECT_STREQ("DefaultMemory", testBank.name());
        EXPECT_EQ(0x10000, testBank.memorySize());
        EXPECT_EQ(0x0000,  testBank.lowAddress());
        EXPECT_EQ(0xFFFF,  testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceDefaultSize)
    {
        // --- given
        // --- when
        Memory testBank( "TestMem" );

        // --- then
        // EXPECT_TRUE(nullptrptr != memory);
        EXPECT_STREQ("TestMem", testBank.name());
        EXPECT_EQ(0x10000, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0xFFFF, testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory testBank( 0x3FF );

        // --- then
        EXPECT_STREQ("DefaultMemory", testBank.name());
        EXPECT_EQ(0x0400, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0x03FF, testBank.highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory testBank( "ThreePages", 0x03FF);

        // --- then
        EXPECT_STREQ("ThreePages", testBank.name());
        EXPECT_EQ(0x0400, testBank.memorySize());
        EXPECT_EQ(0x0000, testBank.lowAddress());
        EXPECT_EQ(0x03FF, testBank.highAddress());
    }

    // ===== Memory Access =====

    // --- Boundaries
    TEST_F(MemoryTest, AccessDefaultMemoryAtBounds)
    {
        // --- given
        // --- when
        // memory.showMemory(0x0000, 0x08, "Start of memory");
        // memory.showMemory(0xFFF8, 0x08, "End of memory");

        // --- then
        EXPECT_EQ(0xAA, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0xFFFF]);
    }

    /// --- operator []
    TEST_F(MemoryTest, ArrayOperatorAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory[0x0000]);

        // --- when
        memory[0x0000] = 0x77;

        // --- then
        EXPECT_EQ(0x77, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0x0001]);
    }


    // --- write (byte)
    TEST_F(MemoryTest, WriteByteAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0x0000));

        // --- when
        memory.write(0x0000, 0x55);

        // --- then
        EXPECT_EQ(0x55, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0x0001]);
    }

    TEST_F(MemoryTest, WriteByteAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory[0xFFFF]);

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory[0xFFFF]);
        EXPECT_EQ(0xAA, memory[0xFFFE]);
    }

    // --- write (word)
    TEST_F(MemoryTest, WriteWordAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0x0000));
        // 0x0000: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeWord(0x0000, 0x5511);

        // 0x0000: 11 55 AA AA AA AA AA AA

        // --- then
        // memory.showMemory(0x0000, 0x08);
        EXPECT_EQ(0x5511, memory.readWord(0x0000));
        EXPECT_EQ(0xAAAA, memory.readWord(0x0002));
        EXPECT_EQ(0xAA55, memory.readWord(0x0001));
    }

    TEST_F(MemoryTest, WriteWordAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFE));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeWord(0xFFFE, 0x7788);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // memory.showMemory(0xFFF0, 0x0F);
        EXPECT_EQ(0x7788, memory.readWord(0xFFFE));
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFC));
        EXPECT_EQ(0x88AA, memory.readWord(0xFFFD));
    }

    TEST_F(MemoryTest, WriteWordWraps)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFF));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeWord(0xFFFF, 0x7788);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // memory.showMemory(0x0000, 0x08);
        // memory.showMemory(0xFFF8, 0x08);

        EXPECT_EQ( 0x88, memory[0xFFFF] );
        EXPECT_EQ( 0x77, memory[0x0000] );
    }

    TEST_F(MemoryTest, WriteAddressWraps)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFF));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeAddress(0xFFFF, 0x5566);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // memory.showMemory(0x0000, 0x08);
        // memory.showMemory(0xFFF8, 0x08);

        EXPECT_EQ( 0x66, memory[0xFFFF] );
        EXPECT_EQ( 0x55, memory[0x0000] );
    }

    // --- read (byte)
    TEST_F(MemoryTest, ReadByeAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0xFFFF));

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory.read(0xFFFF));
        EXPECT_EQ(0xAA, memory.read(0xFFFE));
    }

    // --- read (word)


    // ===============================================================
    // === Banked memory (lower bound is non-zero)
    TEST_F(MemoryTest, ConstructNamedBankedMemory)
    {
        // --- given
        // --- when
        Memory testMemory("0x8000-0x9fff", hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        EXPECT_STREQ("0x8000-0x9fff", testMemory.name());
        EXPECT_EQ(0x2000, testMemory.memorySize()); // 0x9FFF - 0x8000 + 1
        EXPECT_EQ(0x8000, testMemory.lowAddress());
        EXPECT_EQ(0x9FFF, testMemory.highAddress());
    }

    // ===== Memory Access =====

    // --- Boundaries

    TEST_F(MemoryTest, ReadBankedMemoryAtUpperAndLowerBounds)
    {
        // --- given
        // --- when
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0xCC);

        // --- then
        EXPECT_EQ(0xCC, testMemory.read(0x8000));
        EXPECT_EQ(0xCC, testMemory.read(0x9FFF));
    }


    // --- write (byte)

    // --- write (word)
    TEST_F(MemoryTest, WriteWordAtStartOfBankedMemory)
    {
        // --- given
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0xAA);
        EXPECT_EQ(0xAAAA, testMemory.readWord(0x8000));
        // 0x0000: AA AA AA AA AA AA AA AA

        // --- when
        testMemory.writeWord(0x8000, 0x5511);

        // 0x0000: 11 55 AA AA AA AA AA AA

        // --- then
        // testMemory.showMemory(0x8000, 0x08);
        EXPECT_EQ(0x5511, testMemory.readWord(0x8000));
        EXPECT_EQ(0xAA55, testMemory.readWord(0x8001));
        EXPECT_EQ(0xAAAA, testMemory.readWord(0x8002));
    }

    TEST_F(MemoryTest, WriteWordAtEndOfBankedMemory)
    {
        // --- given
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0xAA);
        // EXPECT_EQ(0xAAAA, memory.readWord(0xFFFE));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        testMemory.writeWord(0x9FFE, 0x7788);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // testMemory.showMemory(0x9FF0, 0x0F);
        EXPECT_EQ(0x7788, testMemory.readWord(0x9FFE));
        EXPECT_EQ(0xAAAA, testMemory.readWord(0x9FFC));
        EXPECT_EQ(0x88AA, testMemory.readWord(0x9FFD));
    }

    TEST_F(MemoryTest, BankedMemoryWriteWordWraps)
    {
        // --- given
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0x11);
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        testMemory.writeWord(0x9FFF, 0x7788);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // testMemory.showMemory(0x8000, 0x08, "Wrapped byte");
        // testMemory.showMemory(0x9FF8, 0x08, "end of bank");

        EXPECT_EQ( 0x88, testMemory[0x9FFF] );
        EXPECT_EQ( 0x77, testMemory[0x8000] );
    }

    TEST_F(MemoryTest, BankedMemoryWriteAddressWraps)
    {
        // --- given
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFF));
        // 0xFFF8: AA AA AA AA AA AA AA AA

        // --- when
        memory.writeAddress(0xFFFF, 0x5566);
        // 0xFFF8: AA AA AA AA AA AA 88 77

        // --- then
        // memory.showMemory(0x0000, 0x08);
        // memory.showMemory(0xFFF8, 0x08);

        EXPECT_EQ( 0x66, memory[0xFFFF] );
        EXPECT_EQ( 0x55, memory[0x0000] );
    }


    // --- read (byte)
    TEST_F(MemoryTest, ReadBankedMemoryOutsideOfRangeThrows)
    {
        // --- given
        // --- when
        Memory memoryBank("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        EXPECT_STREQ("0x8000-0x9fff", memoryBank.name());

        // below lower bound
        EXPECT_THROW(memoryBank.read(0x0000), std::out_of_range);
        EXPECT_THROW(memoryBank.read(0x7FFF), std::out_of_range);

        // above upper bound
        EXPECT_THROW(memoryBank.read(0xA000), std::out_of_range);
        EXPECT_THROW(memoryBank.read(0xFFFF), std::out_of_range);
    }

    // --- read (word)

    /// --- operator []
    TEST_F(MemoryTest, SubscriptBankedMemoryAtUpperAndLowerBounds)
    {
        // --- given
        // --- when
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0xBB);

        // --- then
        EXPECT_EQ(0xBB, testMemory[0x8000]);
        EXPECT_EQ(0xBB, testMemory[0x9FFF]);
    }

    TEST_F(MemoryTest, SubscriptBankedMemoryOutsideOfRangeThrows)
    {
        // --- given
        // --- when
        Memory memoryBank("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        EXPECT_STREQ("0x8000-0x9fff", memoryBank.name());

        // Below lower bound
        EXPECT_THROW(memoryBank[0x0000], std::out_of_range);
        EXPECT_THROW(memoryBank[0x7FFF], std::out_of_range);

        // Past Upper bound
        EXPECT_THROW(memoryBank[0xA000], std::out_of_range);
        EXPECT_THROW(memoryBank[0xFFFF], std::out_of_range);
    }

    TEST_F(MemoryTest, DefaultMemoryWriteWordAtUpperBoundWrapsToStartOfBank)
    {
        // --- given
        Memory testMemory("wrap-bank");
        testMemory.fill(0xAA);

        // --- when
        testMemory.writeWord(0xFFFF, 0x7788);

        // --- then
        EXPECT_EQ(0x88, testMemory.read(0xFFFF));
        EXPECT_EQ(0x77, testMemory.read(0x0000));
        EXPECT_EQ(0xAA, testMemory.read(0x0001));
    }

    TEST_F(MemoryTest, BankedMemoryWriteWordAtUpperBoundWrapsToStartOfBank)
    {
        // --- given
        Memory testMemory("0x8000-0x9fff",
                          hardware::Address(0x8000), hardware::Address(0x9FFF));
        testMemory.fill(0x11);

        // --- when
        testMemory.writeWord(0x9FFF, 0x7788);

        // --- then
        EXPECT_EQ(0x88, testMemory.read(0x9FFF));
        EXPECT_EQ(0x77, testMemory.read(0x8000));
        EXPECT_EQ(0x11, testMemory.read(0x8001));
    }

    TEST_F(MemoryTest, BankedMemoryRejectsInvertedRange)
    {
        // --- given / when / then
        EXPECT_THROW(
            Memory invalidBank("bad-range", hardware::Address(0x9000), hardware::Address(0x8000)),
            std::out_of_range);
    }


}
