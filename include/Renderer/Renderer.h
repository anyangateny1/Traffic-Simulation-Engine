#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

struct RenderData;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
  public:
    Renderer(QWidget* parent = nullptr);
    ~Renderer();

    void updateFromRenderData(const RenderData& data);

  protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

  private:
    QOpenGLShaderProgram* shaderProgram = nullptr;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint lineVAO = 0, lineVBO = 0;

    RenderData* renderData_ = nullptr;
    
    void updateLineBuffers();
};
