#ifndef FXGUILD_OMNIMULATOR_PPU_SCREENBUFFER
#define FXGUILD_OMNIMULATOR_PPU_SCREENBUFFER

/*================================================================================================\\
| Image data buffer that is drawn on the screen
|--------------------------------------------------------------------------------------------------|
| Contains a method for editing buffer's pixels.
\=================================================================================================*/

#include <vector>

#include <FXG/Common/Vector2D.h>

namespace FXG { namespace Omni { namespace PPU {

typedef FXG::Common::Vector2D<uint16_t> WinDim;
typedef FXG::Common::Vector2D<uint16_t> WinCoord;

enum class ColorChannel
{
    RED   = 0, 
    GREEN = 1, 
    BLUE  = 2
};

class ScreenBuffer
{
public:

    explicit ScreenBuffer(WinDim i_Dim);

    void setPixelValue(WinCoord i_Coord, ColorChannel i_Channel, uint8_t i_Value);

    WinDim          getDim()     const { return m_Dim; }
    uint8_t const * getDataPtr() const { return m_ImgData.data(); };

private:

    WinDim const         m_Dim;
    std::vector<uint8_t> m_ImgData;
};

inline ScreenBuffer::ScreenBuffer(WinDim i_Dim) :
    m_Dim     (i_Dim)
  , m_ImgData (i_Dim.x * i_Dim.y * 3)
{

}

inline void ScreenBuffer::setPixelValue(WinCoord i_Coord, ColorChannel i_Channel, uint8_t i_Value)
{
    uint8_t channel = static_cast<uint8_t>(i_Channel);
    m_ImgData[3 * (i_Coord.x + (m_Dim.y - i_Coord.y - 1) * m_Dim.x) + channel] = i_Value;
}

}}}

#endif // FXGUILD_OMNIMULATOR_PPU_SCREENBUFFER