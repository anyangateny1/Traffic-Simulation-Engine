#include "Renderer/Renderer.h"
#include "SimulationEngine/SimulationEngine.h"
#include <QMatrix4x4>
#include <QPainter>

// 2D square vertices (centered at origin, 1x1 size)
const float squareVertices[] = {
    -0.5f, -0.5f, // bottom-left
    0.5f,  -0.5f, // bottom-right
    0.5f,  0.5f,  // top-right
    -0.5f, 0.5f   // top-left
};

const unsigned int squareIndices[] = {
    0, 1, 2, // first triangle
    2, 3, 0  // second triangle
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

    isGLInitialized_ = true;

    if (renderData_) {
        updateLineBuffers();
    }
}

void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void Renderer::updateLineBuffers() {
    if (!renderData_ || !isGLInitialized_)
        return;

    makeCurrent();
    std::vector<float> vertices;

    const float roadWidth = 3.0f;

    for (const auto& road : renderData_->roads) {
        std::vector<QVector2D> points;

        // collect points
        auto getNode = [&](auto id) {
            for (auto& n : renderData_->nodes)
                if (n.id == id)
                    return &n;
            return (RenderData::NodeRenderInfo*)nullptr;
        };

        auto* from = getNode(road.from_id);
        auto* to = getNode(road.to_id);

        if (!from || !to)
            continue;

        points.emplace_back(from->pos.x_coord, from->pos.y_coord);
        for (auto& p : road.curve_points)
            points.emplace_back(p.x_coord, p.y_coord);
        points.emplace_back(to->pos.x_coord, to->pos.y_coord);

        // build rectangles
        for (size_t i = 0; i + 1 < points.size(); ++i) {
            QVector2D A = points[i];
            QVector2D B = points[i + 1];

            QVector2D dir = (B - A).normalized();
            QVector2D normal(-dir.y(), dir.x());
            normal *= roadWidth * 0.5f;

            QVector2D A_L = A + normal;
            QVector2D A_R = A - normal;
            QVector2D B_L = B + normal;
            QVector2D B_R = B - normal;

            // triangle 1
            vertices.insert(vertices.end(), {A_L.x(), A_L.y(), B_L.x(), B_L.y(), B_R.x(), B_R.y()});

            // triangle 2
            vertices.insert(vertices.end(), {A_L.x(), A_L.y(), B_R.x(), B_R.y(), A_R.x(), A_R.y()});
        }
    }

    roadVertexCount_ = static_cast<int>(vertices.size() / 2);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
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
        minX = maxX = static_cast<float>(renderData_->nodes[0].pos.x_coord);
        minY = maxY = static_cast<float>(renderData_->nodes[0].pos.y_coord);
        for (const auto& node : renderData_->nodes) {
            minX = std::min(minX, static_cast<float>(node.pos.x_coord));
            maxX = std::max(maxX, static_cast<float>(node.pos.x_coord));
            minY = std::min(minY, static_cast<float>(node.pos.y_coord));
            maxY = std::max(maxY, static_cast<float>(node.pos.y_coord));
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
        projection.ortho(centerX - halfWidth, centerX + halfWidth, minY, maxY, -1.0f, 1.0f);
    } else {
        // Window is taller than scene - fit to width
        float centerY = (minY + maxY) / 2.0f;
        float halfWidth = sceneWidth / 2.0f;
        float halfHeight = halfWidth / aspectRatio;
        projection.ortho(minX, maxX, centerY - halfHeight, centerY + halfHeight, -1.0f, 1.0f);
    }

    shaderProgram->setUniformValue("projection", projection);


    shaderProgram->setUniformValue("color", QVector3D(0.25f, 0.25f, 0.25f));

    QMatrix4x4 identity;
    shaderProgram->setUniformValue("model", identity);

    glBindVertexArray(lineVAO);
    glDrawArrays(GL_TRIANGLES, 0, roadVertexCount_);
    glBindVertexArray(0);

    // Draw nodes as small squares
    shaderProgram->setUniformValue("color", QVector3D(1.0f, 0.4f, 0.4f));
    for (const auto& nodeInfo : renderData_->nodes) {
        QMatrix4x4 model;
        model.translate(nodeInfo.pos.x_coord, nodeInfo.pos.y_coord);
        model.scale(0.8f);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Draw vehicles as squares
    shaderProgram->setUniformValue("color", QVector3D(0.3f, 0.8f, 1.0f));
    for (const auto& vehicleInfo : renderData_->vehicles) {
        QMatrix4x4 model;
        model.translate(vehicleInfo.pos.x_coord, vehicleInfo.pos.y_coord);
        model.scale(1.2f);
        shaderProgram->setUniformValue("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    glBindVertexArray(0);
    shaderProgram->release();

    drawNodeLabels(projection);
}

void Renderer::drawNodeLabels(const QMatrix4x4& projection) {
    if (!renderData_ || renderData_->nodes.empty()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);

    const int viewportWidth = width();
    const int viewportHeight = height();

    for (const auto& nodeInfo : renderData_->nodes) {
        QVector4D worldPos(nodeInfo.pos.x_coord, nodeInfo.pos.y_coord, 0.0f, 1.0f);
        QVector4D clipPos = projection * worldPos;

        float screenX = (clipPos.x() + 1.0f) * 0.5f * viewportWidth;
        float screenY = (1.0f - clipPos.y()) * 0.5f * viewportHeight;

        QString idText = QString::number(nodeInfo.id.value());
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(idText);
        int textHeight = fm.height();

        int offsetX = 10;
        int offsetY = -12;

        painter.drawText(static_cast<int>(screenX - textWidth / 2 + offsetX),
                         static_cast<int>(screenY + textHeight / 4 + offsetY), idText);
    }

    painter.end();
}

void Renderer::updateFromRenderData(const RenderData& data) {
    delete renderData_;
    renderData_ = new RenderData(data);
    updateLineBuffers();
    update();
}
