#include <Omni/Win/ScreenBuffer.h>

namespace Omni { namespace Win {

ScreenBuffer::ScreenBuffer(WinDim i_Dim) :
    m_Dim     (i_Dim)
  , m_ImgData (i_Dim.x * i_Dim.y * 3)
{
    // TODO: remove this silly initial content
    WinCoord coord;
    for (coord.y = 0; coord.y < m_Dim.y; ++coord.y)
    {
        for (coord.x = 0; coord.x < m_Dim.x; ++coord.x)
        {
            double green = static_cast<double>(coord.x) / static_cast<double>(m_Dim.x) * 255.0;
            double blue  = static_cast<double>(coord.y) / static_cast<double>(m_Dim.y) * 255.0;
            setPixelValue(coord, ColorChannel::RED,   0);
            setPixelValue(coord, ColorChannel::GREEN, static_cast<GLubyte>(green));
            setPixelValue(coord, ColorChannel::BLUE,  static_cast<GLubyte>(blue));
        }
    }
}

}}