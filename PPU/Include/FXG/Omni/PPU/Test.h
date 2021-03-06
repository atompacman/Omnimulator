#ifndef FXGUILD_OMNIMULATOR_PPU_TEST
#define FXGUILD_OMNIMULATOR_PPU_TEST

#include <FXG/Omni/Common/DataTypes.h>
#include <FXG/Omni/Common/Graphic.h>
#include <FXG/Omni/PPU/ScreenBuffer.h>

namespace FXG { namespace Omni { namespace PPU {

Common::ColorRGB const COLORS[] = { {(uint8_t)0,   (uint8_t)0,   (uint8_t)  0},
                                    {(uint8_t)192, (uint8_t)32,  (uint8_t) 32},
                                    {(uint8_t)32,  (uint8_t)192, (uint8_t) 32},
                                    {(uint8_t)32,  (uint8_t)32,  (uint8_t)192} };

inline void drawSprite(uint32_t i_SpriteID, ScreenBuffer & io_Buffer, Common::CHRBank const & i_Bank)
{
    uint16_t const spriteX = i_SpriteID % (io_Buffer.getDim().x / 8);
    uint16_t const spriteY = i_SpriteID / (io_Buffer.getDim().y / 8);

    for (uint8_t y = 0; y < 8; ++y)
    {
        uint8_t byteTableA = i_Bank[i_SpriteID * 16 + y];
        uint8_t byteTableB = i_Bank[i_SpriteID * 16 + y + 8];

        for (uint8_t x = 0; x < 8; ++x)
        {
            uint8_t ax = (byteTableA >> (7 - x)) & 1;
            uint8_t bx = (byteTableB >> (7 - x)) & 1;
            uint8_t colorID = ax + (bx << 1);
            Common::ColorRGB const & color = COLORS[colorID];
            for (uint8_t i = 0; i < 3; ++i)
            {
                WinCoord coord(x + spriteX * 8, y + spriteY * 8);
                io_Buffer.setPixelValue(coord, ColorChannel(i), color[i]);
            }
        }
    }
}

}}}

#endif // FXGUILD_OMNIMULATOR_PPU_TEST