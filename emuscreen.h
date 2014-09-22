#ifndef EMUSCREEN_H
#define EMUSCREEN_H

#include <vector>
#include <iostream>

#include <QGLWidget>
#include <QFont>
#include <QFontDatabase>

#include <unicode/ustring.h>
#include <unicode/unistr.h>

class EmuScreen;
#include "screencomponent.h"

class EmuScreen : public QGLWidget
{
    Q_OBJECT
public:
    explicit EmuScreen(QWidget *parent = 0);
    QFont uniFont;
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    QFontDatabase fontdb;
    std::vector<ScreenComponent> screens;
};

#endif // EMUSCREEN_H
