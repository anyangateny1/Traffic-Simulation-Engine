#include "UI/SpawnVehicleWindow.h"

#include <QComboBox>
#include <QDoubleValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <algorithm>
#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>
#include <qvalidator.h>

namespace {
constexpr double kMinSpeed = 0.0;
constexpr double kMaxSpeed = 300.0;
constexpr double kDefaultSpeed = 50.0;
} // namespace

Q_DECLARE_METATYPE(NodeID)

SpawnVehicleWindow::SpawnVehicleWindow(std::span<const Node> nodes, QWidget* parent)
    : QDialog(parent), nodes_(nodes) {
    setWindowTitle("Spawn Vehicle");
    resize(300, 150);

    QLabel* sourceNodeLabel = new QLabel("Source Node:");
    sourceNode_ = new QComboBox(this);

    QLabel* destinationNodeLabel = new QLabel("Destination Node:");
    destinationNode_ = new QComboBox(this);

    QLabel* vehicleSpeedLabel = new QLabel("Vehicle Speed:");
    vehicleSpeed_ = new QLineEdit(this);
    vehicleSpeed_->setText(QString::number(kDefaultSpeed));
    vehicleSpeed_->setValidator(new QDoubleValidator(kMinSpeed, kMaxSpeed, 1, this));

    for (const auto& node : nodes_) {
        QString nodeText = QString("Node %1").arg(node.Id().value());
        QVariant nodeData = QVariant::fromValue(node.Id());
        sourceNode_->addItem(nodeText, nodeData);
        destinationNode_->addItem(nodeText, nodeData);
    }

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(buttons, &QDialogButtonBox::accepted, this, &SpawnVehicleWindow::validateAndAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(sourceNodeLabel);
    layout->addWidget(sourceNode_);
    layout->addWidget(destinationNodeLabel);
    layout->addWidget(destinationNode_);
    layout->addWidget(vehicleSpeedLabel);
    layout->addWidget(vehicleSpeed_);
    layout->addWidget(buttons);
}

SpawnVehicleWindow::~SpawnVehicleWindow() {}

VehicleInfo SpawnVehicleWindow::getVehicleParameters() const {
    NodeID sourceId = sourceNode_->currentData().value<NodeID>();
    NodeID destId = destinationNode_->currentData().value<NodeID>();

    double speed = vehicleSpeed_->text().toDouble();
    speed = std::clamp(speed, kMinSpeed, kMaxSpeed);

    return VehicleInfo{sourceId, destId, speed};
}

bool SpawnVehicleWindow::isValid() const {
    if (sourceNode_->currentIndex() < 0 || destinationNode_->currentIndex() < 0) {
        return false;
    }

    NodeID sourceId = sourceNode_->currentData().value<NodeID>();
    NodeID destId = destinationNode_->currentData().value<NodeID>();
    if (sourceId == destId) {
        return false;
    }
    // Check speed is valid
    bool ok = false;
    double speed = vehicleSpeed_->text().toDouble(&ok);
    if (!ok || speed < kMinSpeed || speed > kMaxSpeed) {
        return false;
    }

    return true;
}

void SpawnVehicleWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (validateAndAccept()) {
            return;
        }
    }
    QDialog::keyPressEvent(event);
}

bool SpawnVehicleWindow::validateAndAccept() {
    if (sourceNode_->currentIndex() < 0 || destinationNode_->currentIndex() < 0) {
        QMessageBox::warning(this, "Invalid Selection",
                             "Please select both source and destination nodes.");
        return false;
    }

    NodeID sourceId = sourceNode_->currentData().value<NodeID>();
    NodeID destId = destinationNode_->currentData().value<NodeID>();
    if (sourceId == destId) {
        QMessageBox::warning(this, "Invalid Selection",
                             "Source and destination nodes must be different.");
        return false;
    }

    bool ok = false;
    vehicleSpeed_->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Speed", "Please enter a valid speed value.");
        return false;
    }

    accept();
    return true;
}
