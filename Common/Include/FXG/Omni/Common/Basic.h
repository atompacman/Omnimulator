#ifndef FXGUILD_OMNIMULATOR_COMMON_BASIC
#define FXGUILD_OMNIMULATOR_COMMON_BASIC

#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <FXG/Omni/Common/Logging.h>

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using sptrc = std::shared_ptr<T const>;

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using uptrc = std::unique_ptr<T const>;

namespace FXG { namespace Omni { namespace Common {

inline uint8_t buildByteFromMSBandLSB(uint8_t i_MSB, uint8_t i_LSB)
{
    return (i_MSB << 4) + i_LSB;
}

inline uint16_t buildLittleEndianAddress(uint8_t i_LSB, uint8_t i_MSB)
{
    return (i_MSB << 8) + i_LSB;
}

}}}

#endif // FXGUILD_OMNIMULATOR_COMMON_BASIC