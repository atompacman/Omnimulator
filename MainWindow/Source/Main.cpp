#include <string>

#include <freeglut/freeglut.h>

const std::string WIN_TITLE  = "Omnimulator";
const uint16_t    WIN_POS_X  = 200;
const uint16_t    WIN_POS_Y  = 200;
const uint16_t    WIN_WIDTH  = 256;
const uint16_t    WIN_HEIGHT = 224;

GLubyte g_TexData[WIN_HEIGHT][WIN_WIDTH][3];;

void displayHandler()
{
    glDrawPixels(WIN_WIDTH, WIN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, g_TexData);
    glutSwapBuffers();
}

void idleHandler()
{
    uint16_t randX = static_cast<uint16_t>(static_cast<double>(rand()) / 
                                           static_cast<double>(RAND_MAX) * WIN_WIDTH);
    uint16_t randY = static_cast<uint16_t>(static_cast<double>(rand()) / 
                                           static_cast<double>(RAND_MAX) * WIN_HEIGHT);
    g_TexData[randY][randX][0] = 255;
    glutPostRedisplay();
}

int main(int i_Argc, char ** i_Argv)
{
    // Init freeglut library
    glutInit(&i_Argc, i_Argv);

    // Init window
    glutInitWindowPosition(WIN_POS_X, WIN_POS_Y);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow(WIN_TITLE.c_str());
    glutDisplayFunc(displayHandler);
    glutIdleFunc(idleHandler);

    // Create bitmap texture   
    for (uint16_t y = 0; y < WIN_HEIGHT; ++y)
    {
        for (uint16_t x = 0; x < WIN_WIDTH; ++x)
        {
            GLubyte * pxl = g_TexData[y][x];
            pxl[0] = 0;
            pxl[1] = static_cast<GLubyte>(static_cast<double>(x) / 
                                          static_cast<double>(WIN_WIDTH) * 255.0);
            pxl[2] = static_cast<GLubyte>(static_cast<double>(y) / 
                                          static_cast<double>(WIN_HEIGHT) * 255.0);
      }
    }

    // Start main loop
    glutMainLoop();

    return EXIT_SUCCESS;
}