#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationConfig.h"
#include "SimulationEngine/SimulationController.h"
#include "SimulationEngine/SimulationEngine.h"
#include "UI/MainWindow.h"
#include "UI/SpawnVehicleWindow.h"
#include "Vehicle/VehicleInfo.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QShowEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <filesystem>


MainWindow::MainWindow(const std::filesystem::path& mapFilePath, QWidget* parent)
    : QMainWindow(parent) {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    layout_ = new QVBoxLayout(centralWidget_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    auto makeButton = [this](const QString& text) { return new QPushButton(text, this); };

    startButton_ = makeButton("Start");
    pauseButton_ = makeButton("Pause");
    stepButton_ = makeButton("Step");
    spawnVehicleButton_ = makeButton("Spawn Vehicle");

    pauseButton_->setEnabled(false);

    buttonLayout->addWidget(startButton_);
    buttonLayout->addWidget(pauseButton_);
    buttonLayout->addWidget(stepButton_);
    buttonLayout->addWidget(spawnVehicleButton_);

    buttonLayout->addStretch();

    layout_->addLayout(buttonLayout);

    try {
        simulationController_ = std::make_unique<SimulationController>(
            std::make_unique<SimulationEngine>(), mapFilePath);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Map Loading Error",
                              QString("Failed to load map: %1").arg(e.what()));
        throw;
    }

    renderer_ = new Renderer(this);
    layout_->addWidget(renderer_);

    simulationTimer_ = new QTimer(this);
    simulationTimer_->setInterval(SimulationConfig::GetTimerIntervalMs());

    connect(startButton_, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(pauseButton_, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(stepButton_, &QPushButton::clicked, this, &MainWindow::onStepClicked);
    connect(spawnVehicleButton_, &QPushButton::clicked, this, &MainWindow::onSpawnVehicleClicked);

    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::onSimulationTick);
    connect(simulationController_.get(), &SimulationController::stateChanged, this,
            &MainWindow::onSimulationStateChanged);
    connect(simulationController_.get(), &SimulationController::renderDataUpdated, renderer_,
            &Renderer::updateFromRenderData);

    simulationController_->emitInitialRenderData();

    setWindowTitle("Traffic Simulation");
    resize(800, 600);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
}

MainWindow::~MainWindow() {}

void MainWindow::onStartClicked() {
    simulationController_->start();
}

void MainWindow::onPauseClicked() {
    simulationController_->pause();
}

void MainWindow::onStepClicked() {
    simulationController_->step();
}

void MainWindow::onSimulationTick() {
    simulationController_->step();
}

void MainWindow::onSpawnVehicleClicked() {
    SpawnVehicleWindow dialog(simulationController_->getNodes(), this);
    if (dialog.exec() == QDialog::Accepted) {
        VehicleInfo info = dialog.getVehicleParameters();
        try {
            simulationController_->spawnVehicle(info);
        } catch (...) {
            // TODO: implement actual catching for things mainwindow can handle here
            QMessageBox::critical(this, "Unexpected error",
                                  "Failed to spawn vehicle due to an unexpected error.");
        }
    }
}

void MainWindow::onSimulationStateChanged(SimState state) {
    switch (state) {
        case SimState::RUNNING:
            simulationTimer_->start();
            startButton_->setEnabled(false);
            pauseButton_->setEnabled(true);
            break;
        case SimState::PAUSED:
        case SimState::STOPPED:
            simulationTimer_->stop();
            startButton_->setEnabled(true);
            pauseButton_->setEnabled(false);
            break;
    }
}
