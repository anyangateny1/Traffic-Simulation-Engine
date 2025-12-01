#include "UI/MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSurfaceFormat>
#include <iostream>

int main(int argc, char* argv[]) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path/to/map.json>\n";
        QMessageBox::critical(nullptr, "Error", "Please provide a map file path as argument.\n\nUsage: ./TrafficSimApp <path/to/map.json>");
        return 1;
    }

    QString mapFilePath = argv[1];

    MainWindow window(mapFilePath);
    window.show();

    return app.exec();
}
