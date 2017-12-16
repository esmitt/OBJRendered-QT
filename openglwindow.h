#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>

/**
 * @brief Base class for windows that use OpenGL.
 */
class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    /**
     * @brief OpenGLWindow Create a new window.
     * @param parent The parent window if this window, if any.
     */
    explicit OpenGLWindow(QWindow *parent = 0);

    /**
     * @brief Destroy a window.
     */
    ~OpenGLWindow();

    /**
     * @brief render Renders the window using a QPainter.
     * @param painter The QPainter to use.
     */
    virtual void render(QPainter *painter);

    /**
     * @brief render Renders this window directly.
     */
    virtual void render();

    /**
     * @brief initialize Sets up OpenGL.
     */
    virtual void initialize();

    /**
     * @brief setAnimating Toggles animation on a window.
     * @param animating The animation state to use.
     */
    void setAnimating(bool animating);

public slots:
    /**
     * @brief renderLater Deferred render request.
     */
    void renderLater();

    /**
     * @brief renderNow Immediate render request.
     */
    void renderNow();

protected:
    /**
     * @brief event Event handler.
     * @param event The event to handle.
     * @return Whether the event was handled or not.
     */
    bool event(QEvent *event) override;

    /**
     * @brief exposeEvent Handles when a window becomes visible.
     * @param event The event.
     */
    void exposeEvent(QExposeEvent *event) override;

    /**
     * @brief m_animating The animation state of the window.
     */
    bool m_animating;

private:
    /**
     * @brief m_context The OpenGL rendering context.
     */
    QOpenGLContext *m_context;

    /**
     * @brief m_device The OpenGL rendering device.
     */
    QOpenGLPaintDevice *m_device;
};

#endif // OPENGLWINDOW_H
