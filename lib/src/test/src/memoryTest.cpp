

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "memory.hpp"

namespace memory
{
    class MemoryTest : public testing::Test
    {
    public:
        Memory &memory;

    protected:
    private:
        Memory *memory_ptr;

    protected:
        MemoryTest() : memory(*(new Memory("test suite")))
        {
            // memory = new Memory("test suite");
            // testMemory( *memory );
            memory.fill(0xAA);
        }

        ~MemoryTest() override
        {
            // delete memory;
        }

        // virtual void SetUp() override
        // {
        //     // memory = new Memory("test suite");
        // }
    };

    TEST_F(MemoryTest, ConstructDefaultAddressSpace)
    {
        // --- given
        // --- when
        Memory *memory = new Memory();

        // --- then
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("DefaultMemory", memory->name());
        EXPECT_EQ(0x10000, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0xFFFF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceDefaultSize)
    {
        // --- given
        // --- when
        Memory *memory = new Memory("TestMem");

        // --- then
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("TestMem", memory->name());
        EXPECT_EQ(0x10000, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0xFFFF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory *memory = new Memory(0x3FF);

        // --- then
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("DefaultMemory", memory->name());
        EXPECT_EQ(0x0400, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0x03FF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithSize)
    {
        // --- given
        // --- when
        Memory *memory = new Memory("ThreePages", 0x3FF);

        // --- then
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("ThreePages", memory->name());
        EXPECT_EQ(0x0400, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0x03FF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithBounds)
    {
        // --- given
        // --- when
        Memory *memory = new Memory("0x8000-0x9fff", hardware::Address(0x8000), hardware::Address(0x9FFF));

        // --- then
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("0x8000-0x9fff", memory->name());
        EXPECT_EQ(0x2000, memory->memorySize()); // 0x9FFF - 0x8000 + 1
        EXPECT_EQ(0x8000, memory->lowAddress());
        EXPECT_EQ(0x9FFF, memory->highAddress());
    }

    // ===== Memory Access =====

    // --- Boundaries
    TEST_F(MemoryTest, AccessDefaultMemoryAtBounds)
    {
        memory.showMemory(0xFFF8, 0x08);

        // --- given
        // --- when
        // --- then
        EXPECT_EQ(0xAA, memory.read(0x0000));
        EXPECT_EQ(0xAA, memory.read(0xFFFF));
    }

    TEST_F(MemoryTest, AccessMemoryNotStartingAtZeroMemoryAtBounds)
    {
        // --- given
        // --- when
        Memory *memory = new Memory("0x8000-0x9fff",
                                    hardware::Address(0x8000), hardware::Address(0x9FFF));
        memory->fill(0xCC);

        // --- then
        EXPECT_EQ(0xCC, memory->read(0x8000));
        EXPECT_EQ(0xCC, memory->read(0x9FFF));
    }

    // --- write (byte)
    TEST_F(MemoryTest, WriteByteAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0x0000));

        // --- when
        memory.write(0x0000, 0x55);
        // --- then

        EXPECT_EQ(0x55, memory.read(0x0000));
        EXPECT_EQ(0xAA, memory.read(0x0001));
    }

    TEST_F(MemoryTest, WriteByteAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0xFFFF));

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory.read(0xFFFF));
        EXPECT_EQ(0xAA, memory.read(0xFFFE));
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
        memory.showMemory(0x0000, 0x08);
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
        memory.showMemory(0xFFF0, 0x0F);
        EXPECT_EQ(0x7788, memory.readWord(0xFFFE));
        EXPECT_EQ(0xAAAA, memory.readWord(0xFFFC));
        EXPECT_EQ(0x88AA, memory.readWord(0xFFFD));
    }

    // TEST_F(MemoryTest, AccessMemoryNotStartingAtZeroMemoryBelowLowerBound)
    // {
    //     Memory *memory = new Memory("0x8000-0x9fff", 0x8000, 0x9FFF);
    //     // EXPECT_TRUE(NULL != memory);
    //     // EXPECT_STREQ("0x8000-0x9fff", memory.name());
    //     EXPECT_EQ(0xFF, memory.read(0x0000));
    //     EXPECT_EQ(0xFF, memory.read(0xFFFF));
    // }

    // --- operator []
    TEST_F(MemoryTest, ArrayOperatorAtStartOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory[0x0000]);

        // --- when
        memory[0x0000] = 0x77;
        // memory[0x0000] = hardware::Byte(0x77);
        // --- then

        EXPECT_EQ(0x77, memory[0x0000]);
        EXPECT_EQ(0xAA, memory[0x0001]);
    }

    TEST_F(MemoryTest, ArrayOperatorAtEndOfMemory)
    {
        // --- given
        EXPECT_EQ(0xAA, memory.read(0xFFFF));

        // --- when
        memory.write(0xFFFF, 0x77);

        // --- then
        EXPECT_EQ(0x77, memory.read(0xFFFF));
        EXPECT_EQ(0xAA, memory.read(0xFFFE));
    }

}
