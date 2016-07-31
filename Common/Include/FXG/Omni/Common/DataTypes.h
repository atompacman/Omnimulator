#ifndef FXGUILD_OMNIMULATOR_COMMON_DATATYPES
#define FXGUILD_OMNIMULATOR_COMMON_DATATYPES

#include <array>
#include <cstdint>

namespace FXG { namespace Omni { namespace Common {

uint32_t const PRG_BANK_SIZE = 1 << 14;
uint32_t const CHR_BANK_SIZE = 1 << 13;
uint32_t const TRAINER_SIZE  = 1 << 9;

typedef std::array<uint8_t, PRG_BANK_SIZE> PRGBank;
typedef std::array<uint8_t, CHR_BANK_SIZE> CHRBank;
typedef std::array<uint8_t, TRAINER_SIZE > Trainer;

}}}

#endif // FXGUILD_OMNIMULATOR_COMMON_DATATYPES