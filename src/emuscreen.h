#ifndef EMUSCREEN_H
#define EMUSCREEN_H

#include <vector>
#include <iostream>

#include <QGLWidget>
#include <QFont>
#include <QFontDatabase>

#include <unicode/ustring.h>
#include <unicode/unistr.h>

#include <FTGL/ftgl.h>
#include <FTGL/FTGLPixmapFont.h>
#include <FTGL/FTPixmapGlyph.h>

class EmuScreen;
#include "screencomponent.h"

class EmuScreen : public QGLWidget
{
    Q_OBJECT
public:
    explicit EmuScreen(QWidget *parent = 0);
    QFont uniFont;
    FTPixmapFont ffont;
    FTBitmapGlyph *glyph;
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    QFontDatabase fontdb;
    std::vector<ScreenComponent> screens;

    FT_Library flib;
    FT_Face fface;
};

#endif // EMUSCREEN_H
