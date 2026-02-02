#pragma once
#include "Road/Node.h"
#include <QDialog>
#include <QKeyEvent>
#include <span>

class SpawnVehicleWindow : public QDialog {
    Q_OBJECT
  public:
    explicit SpawnVehicleWindow(std::span<const Node> nodes, QWidget* parent = nullptr);
    ~SpawnVehicleWindow();

  protected:
    void keyPressEvent(QKeyEvent* event) override;

  private slots:

  private:
    std::span<const Node> nodes_;
};
