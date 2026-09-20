#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class WordTest : public testing::Test
    {
    public:
    protected:
        WordTest()
        {
            // Setup the fixture
        }

        ~WordTest() override
        {
        }

    private:
    };

    // 6502 is little endian - verify the bytes are in the
    // right order
    TEST_F(WordTest, VerifyEndianness)
    {
        // --- given
        // --- when
        hardware::Word word(0x4422);

        // --- then
        EXPECT_EQ(0x4422, word.word);

        EXPECT_EQ(0x44, word.hi);
        EXPECT_EQ(0x22, word.lo);
    }

}