#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <vector>

struct Object {
    float x, y, z;    // Position
    float vx, vy, vz; // Velocity
};

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    Renderer(QWidget* parent = nullptr);
    ~Renderer();

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private slots:
    void updateAnimation();

  private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint VAO = 0, VBO = 0, EBO = 0;

    std::vector<Object> objects; // multiple moving cubes
};
