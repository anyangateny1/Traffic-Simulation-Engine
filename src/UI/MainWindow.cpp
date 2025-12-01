#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationEngine.h"
#include "SimulationEngine/SimulationConfig.h"
#include "UI/MainWindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(const QString& mapFilePath, QWidget* parent) : QMainWindow(parent), isRunning_(false) {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    layout_ = new QVBoxLayout(centralWidget_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    startButton_ = new QPushButton("Start", this);
    pauseButton_ = new QPushButton("Pause", this);
    stepButton_ = new QPushButton("Step", this);

    pauseButton_->setEnabled(false);

    buttonLayout->addWidget(startButton_);
    buttonLayout->addWidget(pauseButton_);
    buttonLayout->addWidget(stepButton_);
    buttonLayout->addStretch();

    layout_->addLayout(buttonLayout);

    simulationEngine_ = new SimulationEngine();

    try {
        simulationEngine_->LoadMap(mapFilePath.toStdString());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Map Loading Error", QString("Failed to load map: %1").arg(e.what()));
        throw;
    }

    renderer_ = new Renderer(this);
    layout_->addWidget(renderer_);

    simulationTimer_ = new QTimer(this);
    simulationTimer_->setInterval(SimulationConfig::GetTimerIntervalMs());

    connect(startButton_, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(pauseButton_, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(stepButton_, &QPushButton::clicked, this, &MainWindow::onStepClicked);

    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::onSimulationTick);

    setWindowTitle("Traffic Simulation");
    resize(800, 600);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    static bool firstShow = true;
    if (firstShow) {
        firstShow = false;
        renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
    }
}

MainWindow::~MainWindow() {
    delete simulationEngine_;
}

void MainWindow::onStartClicked() {
    simulationEngine_->start();
    isRunning_ = true;

    simulationTimer_->start();

    startButton_->setEnabled(false);
    pauseButton_->setEnabled(true);
}

void MainWindow::onPauseClicked() {
    simulationEngine_->pause();
    isRunning_ = false;

    simulationTimer_->stop();

    startButton_->setEnabled(true);
    pauseButton_->setEnabled(false);
}

void MainWindow::onStepClicked() {
    simulationEngine_->step();
    renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}

void MainWindow::onSimulationTick() {
    simulationEngine_->step();
    renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}
