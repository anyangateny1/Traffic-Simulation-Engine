#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationEngine.h"
#include <QMatrix4x4>

// 2D square vertices (centered at origin, 1x1 size)
const float squareVertices[] = {
    -0.5f, -0.5f,  // bottom-left
     0.5f, -0.5f,  // bottom-right
     0.5f,  0.5f,  // top-right
    -0.5f,  0.5f   // top-left
};

const unsigned int squareIndices[] = {
    0, 1, 2,  // first triangle
    2, 3, 0   // second triangle
};

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
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           R"(
        #version 330 core
        layout(location=0) in vec2 aPos;
        uniform mat4 model;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
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
            lineVertices.push_back(fromNode->y);

            // Curve points
            for (const auto& point : roadInfo.curve_points) {
                lineVertices.push_back(point.first);
                lineVertices.push_back(point.second);
            }

            // End point
            lineVertices.push_back(toNode->x);
            lineVertices.push_back(toNode->y);
        }
    }

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(),
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!renderData_) {
        return;
    }

    shaderProgram->bind();

    // Calculate bounds of the scene
    float minX = 0.0f, maxX = 100.0f, minY = 0.0f, maxY = 100.0f;
    if (!renderData_->nodes.empty()) {
        minX = maxX = renderData_->nodes[0].x;
        minY = maxY = renderData_->nodes[0].y;
        for (const auto& node : renderData_->nodes) {
            minX = std::min(minX, static_cast<float>(node.x));
            maxX = std::max(maxX, static_cast<float>(node.x));
            minY = std::min(minY, static_cast<float>(node.y));
            maxY = std::max(maxY, static_cast<float>(node.y));
        }
    }

    // Add padding around the scene
    float padding = 10.0f;
    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;

    // Setup orthographic projection for 2D view
    QMatrix4x4 projection;
    float aspectRatio = float(width()) / float(height());
    float sceneWidth = maxX - minX;
    float sceneHeight = maxY - minY;
    float sceneAspect = sceneWidth / sceneHeight;

    if (aspectRatio > sceneAspect) {
        // Window is wider than scene - fit to height
        float centerX = (minX + maxX) / 2.0f;
        float halfHeight = sceneHeight / 2.0f;
        float halfWidth = halfHeight * aspectRatio;
        projection.ortho(centerX - halfWidth, centerX + halfWidth,
                        minY, maxY, -1.0f, 1.0f);
    } else {
        // Window is taller than scene - fit to width
        float centerY = (minY + maxY) / 2.0f;
        float halfWidth = sceneWidth / 2.0f;
        float halfHeight = halfWidth / aspectRatio;
        projection.ortho(minX, maxX,
                        centerY - halfHeight, centerY + halfHeight, -1.0f, 1.0f);
    }

    shaderProgram->setUniformValue("projection", projection);

    // Draw roads as thick lines
    glLineWidth(4.0f);
    shaderProgram->setUniformValue("color", QVector3D(0.6f, 0.6f, 0.6f));
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

    // Draw nodes as small squares
    shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.4f, 0.4f));
    for (const auto& nodeInfo : renderData_->nodes) {
        QMatrix4x4 model;
        model.translate(nodeInfo.x, nodeInfo.y);
        model.scale(0.8f);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Draw vehicles as squares
    shaderProgram->setUniformValue("color", QVector3D(0.3f, 0.8f, 1.0f));
    for (const auto& vehicleInfo : renderData_->vehicles) {
        QMatrix4x4 model;
        model.translate(vehicleInfo.x, vehicleInfo.y);
        model.scale(1.2f);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
