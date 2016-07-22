#ifndef OMNIMULATOR_COMMON_BASIC
#define OMNIMULATOR_COMMON_BASIC

#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <Omni/Common/Logging.h>

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using sptrc = std::shared_ptr<T const>;

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using uptrc = std::unique_ptr<T const>;

inline uint8_t buildByteFromMSBandLSB(uint8_t i_MSB, uint8_t i_LSB)
{
    return (i_MSB << 4) + i_LSB;
}

#endif // OMNIMULATOR_COMMON_BASIC