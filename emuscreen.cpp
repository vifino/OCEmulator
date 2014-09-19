#include "emuscreen.h"

EmuScreen::EmuScreen(QWidget *parent) :
    QGLWidget(parent)
{
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

    glColor3f(1, 0, 0);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(100, 0);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glEnd();
}
