#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationEngine.h"
#include <QMatrix4x4>

const float cubeVertices[] = {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f, 0.5f,
                              -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
                              -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f, 0.5f};

const unsigned int cubeIndices[] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
                                    1, 5, 6, 6, 2, 1, 3, 2, 6, 6, 7, 3, 0, 1, 5, 5, 4, 0};

Renderer::Renderer(QWidget* parent) : QOpenGLWidget(parent) {}

Renderer::~Renderer() {
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    delete shaderProgram;
    delete renderData_;
    doneCurrent();
}

void Renderer::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos,1.0);
        })");
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           R"(
        #version 330 core
        out vec4 FragColor;
        void main() { FragColor = vec4(0.2,0.7,1.0,1.0); }
        )");
    shaderProgram->link();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (!renderData_) {
        return;
    }
    
    shaderProgram->bind();

    QMatrix4x4 view;
    view.lookAt(QVector3D(0, 10, 0), QVector3D(0, 0, 0), QVector3D(0, 0, -1));

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

    // TODO: Implement road rendering here
    
    for (const auto& vehicleInfo : renderData_->vehicles) {
        QMatrix4x4 model;
        model.translate(vehicleInfo.x, vehicleInfo.y, vehicleInfo.z);
        shaderProgram->setUniformValue("model", model);
        shaderProgram->setUniformValue("view", view);
        shaderProgram->setUniformValue("projection", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
    shaderProgram->release();
}

void Renderer::updateFromRenderData(const RenderData& data) {
    delete renderData_;
    renderData_ = new RenderData(data);
    update();
}
