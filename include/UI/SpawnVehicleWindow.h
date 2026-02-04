#pragma once
#include "Identifiers/IDs.h"
#include "Road/Node.h"
#include "Vehicle/VehicleInfo.h"
#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <qcombobox.h>
#include <span>

class SpawnVehicleWindow : public QDialog {
    Q_OBJECT
  public:
    explicit SpawnVehicleWindow(std::span<const Node> nodes, QWidget* parent = nullptr);
    ~SpawnVehicleWindow();

    VehicleInfo getVehicleParameters() const;
    bool isValid() const;

  protected:
    void keyPressEvent(QKeyEvent* event) override;

  private:
    bool validateAndAccept();
    std::span<const Node> nodes_;

    QComboBox* sourceNode_;
    QComboBox* destinationNode_;
    QLineEdit* vehicleSpeed_;
};
