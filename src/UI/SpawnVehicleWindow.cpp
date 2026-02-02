#include "UI/SpawnVehicleWindow.h"

#include <QComboBox>
#include <QDoubleValidator>
#include <QLabel>
#include <QLineEdit>
#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>
#include <qvalidator.h>

SpawnVehicleWindow::SpawnVehicleWindow(std::span<const Node> nodes, QWidget* parent)
    : nodes_(nodes), QDialog(parent) {
    setWindowTitle("Spawn Vehicle");
    resize(300, 150);

    QLabel* sourceNodeLabel = new QLabel("Source Node:");
    QComboBox* inputSourceNode = new QComboBox();

    QLabel* destinationNodeLabel = new QLabel("Destination Node:");
    QComboBox* inputDestinationNode = new QComboBox();

    QLabel* vehicleSpeedLabel = new QLabel("Vehicle Speed:");
    QLineEdit* inputVehicleSpeed = new QLineEdit(this);
    inputVehicleSpeed->setValidator(new QDoubleValidator(0.0, 300.0, 1, this));


    for (const auto& node : nodes_) {
        QString nodeText = QString("Node %1").arg(node.Id().value());
        inputSourceNode->addItem(nodeText);
        inputDestinationNode->addItem(nodeText);
    }

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(sourceNodeLabel);
    layout->addWidget(inputSourceNode);
    layout->addWidget(destinationNodeLabel);
    layout->addWidget(inputDestinationNode);
    layout->addWidget(vehicleSpeedLabel);
    layout->addWidget(inputVehicleSpeed);
    layout->addWidget(buttons);
}

SpawnVehicleWindow::~SpawnVehicleWindow() {}

void SpawnVehicleWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // TODO: Validate data and store if there is any data
        return;
    }
    QDialog::keyPressEvent(event);
}
