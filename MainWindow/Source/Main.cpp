#include <memory>
#include <string>

#include <freeglut/freeglut.h>

#include <Omni/Common/Common.h>
#include <Omni/Loader/NESROMFile.h>
#include <Omni/Win/ScreenBuffer.h>

namespace Omni { namespace Win {

sptr<ScreenBuffer> g_Buffer;

void handleDisplay()
{
    glDrawPixels(g_Buffer->getDim().x,    // Buffer width
                 g_Buffer->getDim().y,    // Buffer height
                 GL_RGB,                  // 3 channels
                 GL_UNSIGNED_BYTE,        // Data format
                 g_Buffer->getDataPtr()); // Data ptr
    glutSwapBuffers();
}

void handleIdle()
{
    // TODO remove this nonsense
    WinCoord coord;
    coord.x = static_cast<uint16_t>(static_cast<double>(rand()) / 
                                    static_cast<double>(RAND_MAX) * g_Buffer->getDim().x);
    coord.y = static_cast<uint16_t>(static_cast<double>(rand()) / 
                                    static_cast<double>(RAND_MAX) * g_Buffer->getDim().y);
    g_Buffer->setPixelValue(coord, ColorChannel::RED, 255);

    glutPostRedisplay();
}

}}

int main(int i_Argc, char ** i_Argv)
{
    // TODO: move that in a config file or in cmd args
    Omni::Win::WinDim   const WIN_DIM    (256, 224);
    Omni::Win::WinCoord const WIN_POS    (200, 200);
    std::string         const WIN_TITLE  ("Omnimulator");
    std::string         const LOG_CONFIG ("..\\Config\\easyloggingpp.config");
    std::string         const TEST_ROM   ("..\\Resource\\Test\\SMB3.nes");

    // Init logging library
    Omni::Common::initLogging(LOG_CONFIG);
    LOG(INFO) << "Starting Omnimulator";

    // Init freeglut library
    glutInit(&i_Argc, i_Argv);

    // Init window
    glutInitWindowSize     (WIN_DIM.x, WIN_DIM.y);
    glutInitWindowPosition (WIN_POS.x, WIN_POS.y);
    glutInitDisplayMode    (GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow       (WIN_TITLE.c_str());
    glutDisplayFunc        (Omni::Win::handleDisplay);
    glutIdleFunc           (Omni::Win::handleIdle);

    // Create screen buffer
    Omni::Win::g_Buffer = std::make_shared<Omni::Win::ScreenBuffer>(WIN_DIM);

    // Open test .nes file
    Omni::Loader::NESROMFile rom(TEST_ROM);

    // Start main loop
    glutMainLoop();

    return EXIT_SUCCESS;
}