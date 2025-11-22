#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(QWidget* parent = nullptr);

  private:
    QWidget* centralWidget;
    QVBoxLayout* layout;
    QPushButton* startButton;
    QWidget* simulationPanel; // Placeholder for simulation rendering
};
