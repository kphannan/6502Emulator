#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{

    // ===== AddressTest =====
    class AddressTest : public testing::Test
    {
    public:
    protected:
        // AddressTest()
        // {
        //     // Setup the fixture
        // }

        // ~AddressTest() override
        // {
        // }

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
        EXPECT_EQ(0x2211, address);

        EXPECT_EQ(0x22, address.hi());
        EXPECT_EQ(0x11, address.lo());
    }

    TEST_F(AddressTest, AddUnsignedIntToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        // --- when
        address = address + 0x1000;

        // --- then
        EXPECT_EQ(0x3211, address);

        EXPECT_EQ(0x32, address.hi());
        EXPECT_EQ(0x11, address.lo());
    }

    TEST_F(AddressTest, AddByteToAddress)
    {
        // --- given
        hardware::StackAddress address(0x0180);
        hardware::Byte addend(0x14);
        // --- when
        hardware::Address result = address + addend;

        // --- then
        EXPECT_EQ(0x0194, result);
        EXPECT_EQ(0x0180, address);
        EXPECT_EQ(0x14, addend);

        EXPECT_EQ(0x01, address.hi());
        EXPECT_EQ(0x80, address.lo());
    }

    TEST_F(AddressTest, AddWordToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        hardware::Word addend(0x1244);
        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x3455, address);
    }

    TEST_F(AddressTest, AddAddressToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        hardware::Address addend(0x1111);
        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x3322, address);
    }

    // ===== Stack Address =====
    class StackAddressTest : public testing::Test
    {
    public:
    protected:
        // StackAddressTest()
        // {
        //     // Setup the fixture
        // }

        // ~StackAddressTest() override
        // {
        // }

    private:
    };

    // 6502 is little endian - verify the bytes are in the
    // right order
    TEST_F(StackAddressTest, VerifyEndianness)
    {
        // --- given
        // --- when
        hardware::StackAddress address(0x0122);

        // --- then
        EXPECT_EQ(0x0122, address);

        EXPECT_EQ(0x01, address.hi());
        EXPECT_EQ(0x22, address.lo());
    }


    TEST_F(StackAddressTest, AddUnsignedIntToAddress)
    {
        // --- given
        hardware::StackAddress address(0x0134);
        // --- when
        address = address + 0x0010;

        // --- then
        EXPECT_EQ(0x0144, address);

        EXPECT_EQ(0x01, address.hi());
        EXPECT_EQ(0x44, address.lo());
    }

    TEST_F(StackAddressTest, AddByteToAddress)
    {
        // --- given
        hardware::StackAddress address(0x0100);
        hardware::Byte addend(0x14);
        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x0114, address);

        EXPECT_EQ(0x01, address.hi());
        EXPECT_EQ(0x14, address.lo());
    }

    TEST_F(StackAddressTest, AddWordToAddress)
    {
        // --- given
        hardware::StackAddress address(0x0100);
        hardware::Word addend(0x0044);

        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x0144, address);
    }

    TEST_F(StackAddressTest, AddAddressToAddress)
    {
        // --- given
        hardware::StackAddress address(0x0100);
        hardware::Address      addend(0x0055);

        // --- when
        hardware::StackAddress result = address + addend;

        // --- then
        EXPECT_EQ(0x0100, address);
        EXPECT_EQ(0x0055, addend);
        EXPECT_EQ(0x0155, result);
    }


    TEST_F(StackAddressTest, AddAddressToAddressThrows)
    {
        // --- given
        hardware::StackAddress address(0x0100);
        hardware::Address      addend(0x0200);

        // --- when
        EXPECT_THROW(address + addend, std::out_of_range);

        // --- then
    }

    TEST_F(StackAddressTest, StackAddressThrowsZeroPage)
    {
        // --- given

        // --- when
        EXPECT_THROW(hardware::StackAddress(0x0012), std::out_of_range);

        // --- then
    }

    TEST_F(StackAddressTest, StackAddressThrowsPageTwo)
    {
        // --- given

        // --- when
        EXPECT_THROW(hardware::StackAddress(0x02FF), std::out_of_range);

        // --- then
    }



}
