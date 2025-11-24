#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <memory>
#include <vector>

// Forward declarations
class Vehicle;

/**
 * Renderer is a pure View component in the MVC pattern.
 * It receives vehicle data from the controller (MainWindow) and renders it.
 */
class Renderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    Renderer(QWidget* parent = nullptr);
    ~Renderer();

    void updateFromVehicles(const std::vector<std::unique_ptr<Vehicle>>& vehicles);

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // These are non-owning pointers - the controller owns the actual vehicles
    std::vector<const Vehicle*> vehiclesToRender_;
};
