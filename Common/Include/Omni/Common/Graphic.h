#ifndef OMNIMULATOR_COMMON_GRAPHIC
#define OMNIMULATOR_COMMON_GRAPHIC

namespace Omni { namespace Common {

typedef std::array<uint8_t, 3> ColorRGB;

enum class Mirroring
{
    HORIZONTAL,
    VERTICAL,
    FOUR_SCREEN
};

}}

#endif // OMNIMULATOR_COMMON_GRAPHIC