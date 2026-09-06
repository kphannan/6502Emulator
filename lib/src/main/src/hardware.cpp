
#include "hardware.hpp"

hardware::Address hardware::operator+(int val, const hardware::Address &addr)
{
    hardware::Address a = addr.address + val;

    return a;
}
