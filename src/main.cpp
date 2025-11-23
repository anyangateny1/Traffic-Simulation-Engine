#include "UI/MainWindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[]) {
    // Set OpenGL version and profile (required for macOS)
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
