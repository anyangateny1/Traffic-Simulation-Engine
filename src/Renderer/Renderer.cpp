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
    glDeleteBuffers(1, &lineVBO);
    glDeleteVertexArrays(1, &lineVAO);
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
        uniform vec3 color;
        out vec4 FragColor;
        void main() { FragColor = vec4(color, 1.0); }
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

    // Setup line VAO/VBO
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Renderer::updateLineBuffers() {
    if (!renderData_)
        return;

    std::vector<float> lineVertices;

    for (const auto& roadInfo : renderData_->roads) {
        const RenderData::NodeRenderInfo* fromNode = nullptr;
        const RenderData::NodeRenderInfo* toNode = nullptr;

        for (const auto& node : renderData_->nodes) {
            if (node.id == roadInfo.from_id)
                fromNode = &node;
            if (node.id == roadInfo.to_id)
                toNode = &node;
        }

        if (fromNode && toNode) {
            // Start point
            lineVertices.push_back(fromNode->x);
            lineVertices.push_back(0.2f);
            lineVertices.push_back(fromNode->y);

            // Curve points
            for (const auto& point : roadInfo.curve_points) {
                lineVertices.push_back(point.first);
                lineVertices.push_back(0.2f);
                lineVertices.push_back(point.second);
            }

            // End point
            lineVertices.push_back(toNode->x);
            lineVertices.push_back(0.2f);
            lineVertices.push_back(toNode->y);
        }
    }

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(),
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!renderData_) {
        return;
    }

    shaderProgram->bind();

    // Calculate center of all nodes for camera positioning
    float centerX = 0.0f, centerZ = 0.0f;
    if (!renderData_->nodes.empty()) {
        for (const auto& node : renderData_->nodes) {
            centerX += node.x;
            centerZ += node.y;
        }
        centerX /= renderData_->nodes.size();
        centerZ /= renderData_->nodes.size();
    }

    // Position camera to look at the center of the map
    QMatrix4x4 view;
    float cameraHeight = 50.0f;
    view.lookAt(QVector3D(centerX, cameraHeight, centerZ + 30), QVector3D(centerX, 0, centerZ),
                QVector3D(0, 1, 0));

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 200.0f);

    shaderProgram->setUniformValue("view", view);
    shaderProgram->setUniformValue("projection", projection);

    // Draw roads as thick lines
    glLineWidth(8.0f);
    shaderProgram->setUniformValue("color", QVector3D(0.8f, 0.8f, 0.8f));
    QMatrix4x4 identity;
    shaderProgram->setUniformValue("model", identity);

    glBindVertexArray(lineVAO);

    // Draw each road as a line strip
    int vertexOffset = 0;
    for (const auto& roadInfo : renderData_->roads) {
        int pointCount = 2 + roadInfo.curve_points.size();
        glDrawArrays(GL_LINE_STRIP, vertexOffset, pointCount);
        vertexOffset += pointCount;
    }

    glBindVertexArray(0);

    // Draw nodes as cubes
    shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.3f, 0.3f));
    for (const auto& nodeInfo : renderData_->nodes) {
        QMatrix4x4 model;
        model.translate(nodeInfo.x, 0.0f, nodeInfo.y);
        model.scale(0.5f);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    // Draw vehicles as cubes
    shaderProgram->setUniformValue("color", QVector3D(0.2f, 0.7f, 1.0f));
    for (const auto& vehicleInfo : renderData_->vehicles) {
        QMatrix4x4 model;
        model.translate(vehicleInfo.x, vehicleInfo.y, vehicleInfo.z);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
    shaderProgram->release();
}

void Renderer::updateFromRenderData(const RenderData& data) {
    delete renderData_;
    renderData_ = new RenderData(data);
    updateLineBuffers();
    update();
}
