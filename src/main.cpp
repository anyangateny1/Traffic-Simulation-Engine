#include "UI/MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QSurfaceFormat>

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace {
void ShowUsageError(const char* programName, const QString& message) {
    std::cerr << "Usage: " << programName << " <path/to/map.json>\n";
    QMessageBox::critical(nullptr, "Error", message);
}
} // namespace

int main(int argc, char* argv[]) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    if (argc < 2) {
        ShowUsageError(argv[0], "Please provide a map file path as argument.\n\n"
                                "Usage: ./TrafficSimApp <path/to/map.json>");
        return 1;
    }

    std::filesystem::path mapFilePath = argv[1];

    if (!std::filesystem::exists(mapFilePath)) {
        ShowUsageError(
            argv[0],
            QString("File does not exist: %1").arg(QString::fromStdString(mapFilePath.string())));
        return 1;
    }

    if (!std::filesystem::is_regular_file(mapFilePath)) {
        ShowUsageError(argv[0], QString("Path is not a regular file: %1")
                                    .arg(QString::fromStdString(mapFilePath.string())));
        return 1;
    }

    std::string ext = mapFilePath.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (ext != ".json") {
        ShowUsageError(argv[0], QString("File must have .json extension: %1")
                                    .arg(QString::fromStdString(mapFilePath.string())));
        return 1;
    }

    MainWindow window(mapFilePath);
    window.show();

    return app.exec();
}
