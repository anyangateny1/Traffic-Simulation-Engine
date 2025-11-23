#pragma once
#include "Renderer/Renderer.h"
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(QWidget* parent = nullptr);

  private:
    QWidget* centralWidget_;
    QVBoxLayout* layout_;
    QPushButton* startButton_;
    Renderer* renderer_; // Placeholder for simulation rendering
};
