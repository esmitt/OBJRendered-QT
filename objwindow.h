#ifndef OBJWINDOW_H
#define OBJWINDOW_H

#include <QOpenGLShaderProgram>
#include <openglwindow.h>

/**
 * @brief A window class that renders OBJ models.
 */
class ObjWindow : public OpenGLWindow
{
public:
    ObjWindow(QWindow *parent = 0);

    void initialize() override;
    void render() override;

protected:
    bool event(QEvent *event) override;

private:
    // Render attributes
    GLuint m_posAttr; // Position
    GLuint m_norAttr; // Normal
    GLuint m_uvAttr;  // Tex. coordinates.

    // Render uniforms.
    GLuint light_PosDir;
    GLuint ModelViewProyection_Matrix;
    GLuint Model_Matrix;
    GLuint View_Matrix;
    GLuint Normal_Matrix;
    GLuint point_light;
    GLuint AmbientColor_Material;
    GLuint DiffuseColor_Material;
    GLuint SpecularColor_Material;
    GLuint Shinnines_Material;
    GLuint Ambient_Light;
    GLuint Diffuse_Light;
    GLuint Specular_Light;
    GLuint UseMaterial;
    GLuint UseLight;
    GLuint BaseColor;
    GLuint UseGrayscale;

    QOpenGLShaderProgram *m_program;
    int m_frame;
    unsigned int m_modelId;

    //silhouette
    QOpenGLShaderProgram* m_ProgSilhouette;
    GLuint matrixMVP_ID;
    GLuint uniformOffset;
    GLuint vertex_attribute_loc;
    GLuint normal_attribute_loc;

    //shading
    QOpenGLShaderProgram* m_ProgEffect;
    GLuint matrixM_ID;
    GLuint matrixMV_ID;
    GLuint matrixVP_ID;
    GLuint uniformLightPos_ID;
    GLuint vertex_model_ID;
    GLuint normal_model_ID;
    GLuint color_model_ID;
    QVector3D m_lightPos;

    // Enables/disables
    bool enable_sil;
    bool enable_gfx;
    bool enable_grayscale;

    /**
     * @brief ReadShaderFile Reads a GLSL shader from a file.
     * @param strName The shader to read.
     * @return The shader code.
     */
    QString ReadShaderFile(QString strName);
};

#endif // OBJWINDOW_H
