#include <FXG/Omni/Common/Basic.h>
#include <FXG/Omni/Common/DataTypes.h>
#include <FXG/Omni/CPU/Operation.h>

namespace FXG { namespace Omni { namespace CPU {

std::array<sptrc<Operation>, 256> g_Operations;

inline void test(Common::PRGBank const & i_Bank)
{
    /*
    g_Operations[0x18] = std::make_shared<StatusFlagOperation const>(
        "CLC", StatusFlagOperation::Action::CLEAR, StatusFlag::CARRY);
    g_Operations[0x38] = std::make_shared<StatusFlagOperation const>(
        "SEC", StatusFlagOperation::Action::SET, StatusFlag::CARRY);
    g_Operations[0x58] = std::make_shared<StatusFlagOperation const>(
        "CLI", StatusFlagOperation::Action::CLEAR, StatusFlag::INTERRUPT);
    g_Operations[0x78] = std::make_shared<StatusFlagOperation const>(
        "SEI", StatusFlagOperation::Action::SET, StatusFlag::INTERRUPT);
    g_Operations[0xB8] = std::make_shared<StatusFlagOperation const>(
        "CLV", StatusFlagOperation::Action::CLEAR, StatusFlag::OVERFLOW_);
    g_Operations[0xD8] = std::make_shared<StatusFlagOperation const>(
        "CLD", StatusFlagOperation::Action::CLEAR, StatusFlag::DECIMAL_MODE);
    g_Operations[0xF8] = std::make_shared<StatusFlagOperation const>(
        "SED", StatusFlagOperation::Action::SET, StatusFlag::DECIMAL_MODE);
        */
    auto const i = 0xFFFC - 0xC000;
    uint16_t address = (i_Bank[i + 1] << 8) | i_Bank[i];
    uint8_t opCode = i_Bank[address - 0xC000];
}

}}}