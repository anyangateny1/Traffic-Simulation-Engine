#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationEngine.h"
#include "UI/MainWindow.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), isRunning_(false) {
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

    renderer_ = new Renderer(this);
    layout_->addWidget(renderer_);

    simulationTimer_ = new QTimer(this);
    simulationTimer_->setInterval(16); // ~60 FPS (16ms per frame)

    connect(startButton_, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(pauseButton_, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(stepButton_, &QPushButton::clicked, this, &MainWindow::onStepClicked);

    connect(simulationTimer_, &QTimer::timeout, this, &MainWindow::onSimulationTick);

    // TODO: Wire up the new RenderData API
    // renderer_->updateFromRenderData(simulationEngine_->GetRenderData());

    setWindowTitle("Traffic Simulation - MVC Architecture");
    resize(800, 600);
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

    // TODO: Wire up the new RenderData API
    // renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}

void MainWindow::onSimulationTick() {
    simulationEngine_->step();

    // TODO: Wire up the new RenderData API
    // renderer_->updateFromRenderData(simulationEngine_->GetRenderData());
}
