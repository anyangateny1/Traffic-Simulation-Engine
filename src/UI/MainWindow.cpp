#include "UI/MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create vertical layout
    layout = new QVBoxLayout(centralWidget);

    // Create simulation panel (just a blank widget for now)
    simulationPanel = new QWidget(this);
    simulationPanel->setFixedSize(600, 400);                        // placeholder size
    simulationPanel->setStyleSheet("background-color: lightgray;"); // visible background
    layout->addWidget(simulationPanel);

    // Create Start button
    startButton = new QPushButton("Start Simulation", this);
    layout->addWidget(startButton);

    // Connect button to a placeholder slot (later will start simulation)
    connect(startButton, &QPushButton::clicked,
            [this]() { startButton->setText("Simulation Started!"); });

    setWindowTitle("Traffic Simulation MVP");
    resize(640, 480);
}
