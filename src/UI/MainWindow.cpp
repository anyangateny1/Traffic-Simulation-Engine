#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create central widget
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);

    // Create vertical layout
    layout_ = new QVBoxLayout(centralWidget_);

    // Create simulation panel (just a blank widget for now)
    renderer_ = new Renderer(this);
    layout_->addWidget(renderer_);

    // Create Start button
    startButton_ = new QPushButton("Start Simulation", this);
    layout_->addWidget(startButton_);

    // Connect button to a placeholder slot (later will start simulation)
    connect(startButton_, &QPushButton::clicked,
            [this]() { startButton_->setText("Simulation Started!"); });

    setWindowTitle("Traffic Simulation MVP");
    resize(640, 480);
}
