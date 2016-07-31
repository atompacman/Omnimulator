#include <memory>
#include <string>

#include <freeglut/freeglut.h>

#include <FXG/Omni/Loader/NESROMFile.h>
#include <FXG/Omni/PPU/ScreenBuffer.h>
#include <FXG/Omni/PPU/Test.h>

namespace FXG { namespace Omni { namespace Win {

sptr<PPU::ScreenBuffer>  g_Buffer;
sptr<Loader::NESROMFile> g_ROMFile;
uint32_t                 g_CurrSprite = 0;

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
    static uint32_t const MAX_NUM_SPRITES = Common::CHR_BANK_SIZE / 16U;

    if (g_CurrSprite < MAX_NUM_SPRITES)
    {
        drawSprite(g_CurrSprite, *g_Buffer, g_ROMFile->getCHRBanks()[0]);
        glutPostRedisplay();
        ++g_CurrSprite;
    }
}

}}}

int main(int i_Argc, char ** i_Argv)
{
    using namespace FXG::Omni;

    // TODO: move that in a config file or in cmd args
    PPU::WinDim   const WIN_DIM    (256, 224);
    PPU::WinCoord const WIN_POS    (200, 200);
    std::string         const WIN_TITLE  ("Omnimulator");
    std::string         const LOG_CONFIG ("..\\Config\\easyloggingpp.config");
    std::string         const TEST_ROM   ("..\\Resource\\Test\\Donkey_kong.nes");

    // Init logging library
    Common::initLogging(LOG_CONFIG);
    LOG(INFO) << "Starting Omnimulator";

    // Init freeglut library
    glutInit(&i_Argc, i_Argv);

    // Init window
    glutInitWindowSize     (WIN_DIM.x, WIN_DIM.y);
    glutInitWindowPosition (WIN_POS.x, WIN_POS.y);
    glutInitDisplayMode    (GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow       (WIN_TITLE.c_str());
    glutDisplayFunc        (Win::handleDisplay);
    glutIdleFunc           (Win::handleIdle);

    // Create screen buffer
    Win::g_Buffer = std::make_shared<PPU::ScreenBuffer>(WIN_DIM);

    // Open test .nes file
    Win::g_ROMFile = std::make_shared<Loader::NESROMFile>(TEST_ROM);

    // Start main loop
    glutMainLoop();

    return EXIT_SUCCESS;
}