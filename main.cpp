#include <cstdlib>

#include <QApplication>
#include <QSurfaceFormat>

#include "objwindow.h"

int main(int argc, char *argv[])
{
    // Create a QT application object for the GUI.
    QApplication app(argc, argv);

    // Set the window surface parameters
    QSurfaceFormat format;
    format.setSamples(16);

    // Create the OpenGL window
    ObjWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();
    window.setAnimating(true);

    // Run the GUI
    return app.exec();
}
