#include "emuscreen.h"

EmuScreen::EmuScreen(QWidget *parent) :
    QGLWidget(parent)
{
    fontdb.addApplicationFont(":/fonts/unifont.ttf");
    uniFont = fontdb.font("Unifont", "Medium", 8);

    ScreenComponent sc(this);
    screens.push_back(sc);
}


void EmuScreen::initializeGL()
{
    glDisable(GL_DEPTH_TEST);

    qglClearColor(QColor(0, 128, 255));
}

void EmuScreen::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

void EmuScreen::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 1, 0);

    for (std::vector<ScreenComponent>::iterator it = screens.begin(); it != screens.end(); ++it)
    {
        it->draw();
    }
}
