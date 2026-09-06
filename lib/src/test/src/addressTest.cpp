#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class AddressTest : public testing::Test
    {
    public:
    protected:
        AddressTest()
        {
            // Setup the fixture
        }

        ~AddressTest() override
        {
        }

    private:
    };

    // 6502 is little endian - verify the bytes are in the
    // right order
    TEST_F(AddressTest, VerifyEndianness)
    {
        // --- given
        // --- when
        hardware::Address address(0x2211);

        // --- then
        EXPECT_EQ(0x2211, address.address);

        EXPECT_EQ(0x22, address.pch);
        EXPECT_EQ(0x11, address.pcl);
    }

}