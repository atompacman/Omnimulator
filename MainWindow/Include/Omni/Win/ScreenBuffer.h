#ifndef OMNIMULATOR_MAINWINDOW_SCREENBUFFER
#define OMNIMULATOR_MAINWINDOW_SCREENBUFFER

#include <vector>

#include <aplib/Vector2D.h>
#include <freeglut/freeglut.h>

namespace Omni { namespace Win {

typedef Vector2D<uint16_t> WinDim;
typedef Vector2D<uint16_t> WinCoord;

enum class ColorChannel
{
    RED   = 0, 
    GREEN = 1, 
    BLUE  = 2
};

/*================================================================================================\\
| Image data buffer that is drawn on the screen
|--------------------------------------------------------------------------------------------------|
| Contains a method for editing buffer's pixels.
\=================================================================================================*/

class ScreenBuffer
{
public:

    explicit ScreenBuffer(WinDim i_Dim);

    void setPixelValue(WinCoord i_Coord, ColorChannel i_Channel, GLubyte i_Value);

    WinDim          getDim()     const { return m_Dim; }
    GLubyte const * getDataPtr() const { return m_ImgData.data(); };

private:

    WinDim const         m_Dim;
    std::vector<GLubyte> m_ImgData;
};

inline void ScreenBuffer::setPixelValue(WinCoord i_Coord, ColorChannel i_Channel, GLubyte i_Value)
{
    m_ImgData[3 * (i_Coord.x + i_Coord.y * m_Dim.x) + static_cast<uint16_t>(i_Channel)] = i_Value;
}

}}

#endif // OMNIMULATOR_MAINWINDOW_SCREENBUFFER