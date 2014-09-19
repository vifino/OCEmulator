#ifndef EMUSCREEN_H
#define EMUSCREEN_H

#include <QGLWidget>
#include <iostream>

class EmuScreen : public QGLWidget
{
    Q_OBJECT
public:
    explicit EmuScreen(QWidget *parent = 0);

signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
};

#endif // EMUSCREEN_H
