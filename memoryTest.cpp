

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "memory.hpp"
// using namespace memory;

namespace memory
{
    class MemoryTest : public testing::Test
    {
    public:
        Memory *memory;

    protected:
        MemoryTest()
        {
            memory = new Memory("test suite");
        }

        ~MemoryTest() override
        {
            delete memory;
        }

        // virtual void SetUp() override
        // {
        //     // memory = new Memory("test suite");
        // }
    };

    TEST_F(MemoryTest, ConstructDefaultAddressSpace)
    {
        Memory *memory = new Memory();
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("DefaultMemory", memory->name());
        EXPECT_EQ(0xFFFF, memory->memorySize());
        EXPECT_EQ(0xFFFF, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0xFFFF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceDefaultSize)
    {
        Memory *memory = new Memory("TestMem");
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("TestMem", memory->name());
        EXPECT_EQ(0xFFFF, memory->memorySize());
        EXPECT_EQ(0xFFFF, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0xFFFF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructAddressSpaceWithSize)
    {
        Memory *memory = new Memory(0x3FF);
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("DefaultMemory", memory->name());
        EXPECT_EQ(0x03FF, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0x03FF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithSize)
    {
        Memory *memory = new Memory("ThreePages", 0x3FF);
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("ThreePages", memory->name());
        EXPECT_EQ(0x03FF, memory->memorySize());
        EXPECT_EQ(0x0000, memory->lowAddress());
        EXPECT_EQ(0x03FF, memory->highAddress());
    }

    TEST_F(MemoryTest, ConstructNamedAddressSpaceWithBounds)
    {
        Memory *memory = new Memory("0x8000-0x9fff", 0x8000, 0x9FFF);
        EXPECT_TRUE(NULL != memory);
        EXPECT_STREQ("0x8000-0x9fff", memory->name());
        EXPECT_EQ(0x1FFF, memory->memorySize());
        EXPECT_EQ(0x8000, memory->lowAddress());
        EXPECT_EQ(0x9FFF, memory->highAddress());
    }

}
