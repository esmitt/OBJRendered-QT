#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <QMatrix4x4>
#include <QVector3D>
#include <QScreen>
#include <QFile>
#include <QtCore>
#include <QKeyEvent>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>

#include "objwindow.h"
#include "objfactory.h"

ObjWindow::ObjWindow(QWindow *parent)
    : OpenGLWindow(parent)
    , m_program(0)
    , m_frame(0)
    , enable_sil(true)
    , enable_gfx(true)
    , enable_grayscale(false)
{
    m_ProgSilhouette = NULL;
    m_ProgEffect = NULL;
    m_lightPos.setX(0.1f);
    m_lightPos.setY(0.5f);
    m_lightPos.setZ(1.0f);
}

void ObjWindow::initialize()
{
    OpenGLWindow::initialize();

    // Silhouette
    m_ProgSilhouette = new QOpenGLShaderProgram(this);
    m_ProgSilhouette->addShaderFromSourceFile(QOpenGLShader::Vertex, (QFileInfo(".").absolutePath() + "/ObjRenderer/silhouette.vert").toStdString().c_str());
    m_ProgSilhouette->addShaderFromSourceFile(QOpenGLShader::Fragment, (QFileInfo(".").absolutePath() + "/ObjRenderer/silhouette.frag").toStdString().c_str());
    m_ProgSilhouette->link();

    // Get uniform and attribute locations
    matrixMVP_ID = m_ProgSilhouette->uniformLocation("MVP");
    uniformOffset = m_ProgSilhouette->uniformLocation("offset");
    vertex_attribute_loc = m_ProgSilhouette->attributeLocation("position");
    normal_attribute_loc = m_ProgSilhouette->attributeLocation("normal");

    // Effect shader
    m_ProgEffect = new QOpenGLShaderProgram(this);
    m_ProgEffect->addShaderFromSourceFile(QOpenGLShader::Vertex, (QFileInfo(".").absolutePath() + "/ObjRenderer/VertexShader.vert").toStdString().c_str());
    m_ProgEffect->addShaderFromSourceFile(QOpenGLShader::Fragment, (QFileInfo(".").absolutePath() + "/ObjRenderer/FragmentShader.frag").toStdString().c_str());
    m_ProgEffect->link();

    // Get uniform and attribute locations
    matrixM_ID = m_ProgEffect->uniformLocation("M");
    matrixMV_ID = m_ProgEffect->uniformLocation("MV");
    matrixVP_ID = m_ProgEffect->uniformLocation("P");
    uniformLightPos_ID = m_ProgEffect->uniformLocation("LightPos");
    vertex_model_ID = m_ProgEffect->attributeLocation("position");
    normal_model_ID = m_ProgEffect->attributeLocation("normal");
    color_model_ID = m_ProgEffect->attributeLocation("color");

    // Render shader
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, (QFileInfo(".").absolutePath() + "/ObjRenderer/v1.vert").toStdString().c_str());
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, (QFileInfo(".").absolutePath() + "/ObjRenderer/v1.frag").toStdString().c_str());
    m_program->link();

    // Get uniform and attribute locations
    m_posAttr = m_program->attributeLocation("vertexPosition");
    m_norAttr = m_program->attributeLocation("vertexNormal");
    m_uvAttr = m_program->attributeLocation("vertexUV");
    light_PosDir = m_program->uniformLocation("light_PosDir");
    ModelViewProyection_Matrix = m_program->uniformLocation("ModelViewProyection_Matrix");
    Model_Matrix = m_program->uniformLocation("Model_Matrix");
    View_Matrix = m_program->uniformLocation("View_Matrix");
    Normal_Matrix = m_program->uniformLocation("Normal_Matrix");
    point_light = m_program->uniformLocation("point_light");
    AmbientColor_Material = m_program->uniformLocation("AmbientColor_Material");
    DiffuseColor_Material = m_program->uniformLocation("DiffuseColor_Material");
    SpecularColor_Material = m_program->uniformLocation("SpecularColor_Material");
    Shinnines_Material = m_program->uniformLocation("Shinnines_Material");
    Ambient_Light = m_program->uniformLocation("Ambient_Light");
    Diffuse_Light = m_program->uniformLocation("Diffuse_Light");
    Specular_Light = m_program->uniformLocation("Specular_Light");
    UseMaterial = m_program->uniformLocation("UseMaterial");
    UseLight = m_program->uniformLocation("UseLight");
    BaseColor = m_program->uniformLocation("BaseColor");
    UseGrayscale = m_program->uniformLocation("UseGrayscale");

    // Try to load the model.
    try {
        m_modelId = ObjFactory::loadObj((QFileInfo(".").absolutePath() + "/ObjRenderer/model").toStdString().c_str(), "suzane.obj");
    } catch (std::string s) {
        std::cerr << s << std::endl;
        exit(EXIT_FAILURE);
    }
    // Set the model's uniform locations.
    ObjFactory::setUniforms(m_modelId, AmbientColor_Material, DiffuseColor_Material, SpecularColor_Material, Shinnines_Material);

    //inital setup
    glEnable(GL_DEPTH_TEST);
}

void ObjWindow::render()
{
    // Set the OpenGL viewport in case the window changed.
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    // Clear the screen and OpenGL state.
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up a perspective projection matrix.
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(60.0f, width()/height(), 0.01f, 100.0f);

    // Set up a view matrix looking towards (0, 0, 0) from (0, 0, 1).
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0, 0, 1), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    // Set up a model matrix to rotate.
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.rotate(10.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    // Multiply the previous matrix to obtain the model-view-projection matrix.
    QMatrix4x4 mvpMatrix;
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    // If the silhouette effect is enabled.
    if (enable_sil) {
        // Enable the silhouette shader.
        m_ProgSilhouette->bind();
        m_ProgSilhouette->setUniformValue(matrixMVP_ID, mvpMatrix);
        m_ProgSilhouette->setUniformValue(uniformOffset, 0.02f);
        // Draw enlarge mesh
        glCullFace(GL_CW); // Front face culling.
        glDepthMask(GL_FALSE); // Enables writes in z-buffer.
        ObjFactory::renderObj(m_modelId);
        // Disable the shader.
        m_ProgSilhouette->release();

        // Restore GL state.
        glDepthMask(GL_TRUE);
        glCullFace(GL_CCW);
    }

    // If the effect is enabled.
    if (enable_gfx) {
        // Set shader and uniforms.
        m_ProgEffect->bind();
        m_ProgEffect->setUniformValue(matrixM_ID, modelMatrix);
        m_ProgEffect->setUniformValue(matrixMV_ID, viewMatrix * modelMatrix);
        m_ProgEffect->setUniformValue(matrixVP_ID, projectionMatrix * viewMatrix);
        m_ProgEffect->setUniformValue(uniformLightPos_ID, m_lightPos);
        // Render the model.
        ObjFactory::renderObj(m_modelId);
        // Disable the shader.
        m_ProgEffect->release();
    }

    // Enable the shader.
    m_program->bind();

    // Set up the normal matrix.
    QMatrix4x4 normalMatrix;
    normalMatrix = viewMatrix * modelMatrix;
    normalMatrix.inverted().transposed();

    // Set uniforms.
    m_program->setUniformValue(ModelViewProyection_Matrix, mvpMatrix);
    m_program->setUniformValue(Model_Matrix, modelMatrix);
    m_program->setUniformValue(View_Matrix, viewMatrix);
    m_program->setUniformValue(Normal_Matrix, normalMatrix);
    m_program->setUniformValue(UseLight, true);
    m_program->setUniformValue(UseMaterial, enable_gfx);
    m_program->setUniformValue(UseGrayscale, enable_grayscale);
    m_program->setUniformValue(point_light, true);
    m_program->setUniformValue(light_PosDir, QVector3D(0,1,3));
    m_program->setUniformValue(Shinnines_Material, 128);

    // Render the model
    ObjFactory::renderObj(m_modelId);

    // Disable the shader
    m_program->release();

    // Advance the animation.
    if (m_animating)
        ++m_frame;
}

bool ObjWindow::event(QEvent *event)
{
    QKeyEvent * keyEvent;
    QString path = "";
    QFileInfo info;

    // Check the event type.
    switch (event->type()) {
    // If the event was a key press.
    case QEvent::KeyPress:
        // Convert the event to a key event.
        keyEvent= dynamic_cast<QKeyEvent *>(event);
        assert(keyEvent != nullptr);

        // Check which key was pressed.
        switch (keyEvent->key()) {
        case Qt::Key_L:
            // If it is the L key then ask for a model to load.
            path = QFileDialog::getOpenFileName(nullptr, "Open Model", "C:/", "Obj Files (*.obj)");
            info = QFileInfo(path);
            qDebug() << info.absolutePath() << " --> " << info.fileName();

            // Try to load the new model file.
            try {
                ObjFactory::unloadObj(m_modelId);
                m_modelId = ObjFactory::loadObj(info.absolutePath().toStdString().c_str(), info.fileName().toStdString().c_str());
            } catch (std::string s) {
                std::cerr << s << std::endl;
                exit(EXIT_FAILURE);
            }
            // Set the uniform locations for the new model.
            ObjFactory::setUniforms(m_modelId, AmbientColor_Material, DiffuseColor_Material, SpecularColor_Material, Shinnines_Material);
            break;

        case Qt::Key_T:
            // Toggle the effect or normal rendering.
            enable_gfx = !enable_gfx;
            break;

        case Qt::Key_S:
            // Toggle the silhouette rendering.
            enable_sil = !enable_sil;
            break;

        case Qt::Key_G:
            // Toggle grayscale rendering.
            enable_grayscale = !enable_grayscale;
            break;
        }
        return true;

    // If the event is not a key event then pass it to the superclass.
    default:
        keyEvent = nullptr;
        return OpenGLWindow::event(event);
    }
}
