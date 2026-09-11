#include <gtest/gtest.h>
// #include <gmock/gmock.h>

#include "6502.hpp"
#include "memory.hpp"

namespace m6502
{
    class AddressOldTest : public testing::Test
    {
    public:
    protected:
        AddressOldTest()
        {
            // Setup the fixture
        }

        ~AddressOldTest() override
        {
        }

    private:
    };

    // 6502 is little endian - verify the bytes are in the
    // right order
    TEST_F(AddressOldTest, VerifyEndianness)
    {
        // --- given
        // --- when
        hardware::Address address(0x2211);

        // --- then
        EXPECT_EQ(0x2211, address);

        EXPECT_EQ(0x22, address.hi());
        EXPECT_EQ(0x11, address.lo());
    }

    // ===== AddressTest =====
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
        StackAddressTest()
        {
            // Setup the fixture
        }

        ~StackAddressTest() override
        {
        }

    private:
    };

    // 6502 is little endian - verify the bytes are in the
    // right order
    TEST_F(StackAddressTest, VerifyEndianness)
    {
        // --- given
        // --- when
        hardware::Address address(0x2211);

        // --- then
        EXPECT_EQ(0x2211, address);

        EXPECT_EQ(0x22, address.hi());
        EXPECT_EQ(0x11, address.lo());
    }

    TEST_F(StackAddressTest, AddUnsignedIntToAddress)
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

    TEST_F(StackAddressTest, AddByteToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        hardware::Byte addend(0x14);
        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x2225, address);

        EXPECT_EQ(0x22, address.hi());
        EXPECT_EQ(0x25, address.lo());
    }

    TEST_F(StackAddressTest, AddWordToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        hardware::Word addend(0x1244);
        // --- when
        address = address + addend;

        // --- then
        EXPECT_EQ(0x3455, address);
    }

    TEST_F(StackAddressTest, AddAddressToAddress)
    {
        // --- given
        hardware::Address address(0x2211);
        hardware::Address addend(0x1111);
        // --- when
        hardware::Address result = address + addend;
        // address = address + addend;

        // --- then
        EXPECT_EQ(0x2211, address);
        EXPECT_EQ(0x1111, addend);
        EXPECT_EQ(0x3322, result);
    }

}
