#ifndef FXGUILD_OMNIMULATOR_COMMON_GRAPHIC
#define FXGUILD_OMNIMULATOR_COMMON_GRAPHIC

#include <array>

namespace FXG { namespace Omni { namespace Common {

typedef std::array<uint8_t, 3> ColorRGB;

enum class Mirroring
{
    HORIZONTAL,
    VERTICAL,
    FOUR_SCREEN
};

}}}

#endif // FXGUILD_OMNIMULATOR_COMMON_GRAPHIC