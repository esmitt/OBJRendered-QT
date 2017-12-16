#include <iostream>
#include <cstdlib>

#include <QPainter>
#include<QCoreApplication>

#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
    , m_animating(true)
    , m_context(0)
    , m_device(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{

}

void OpenGLWindow::render()
{
    // Initialize a paint device if there is none.
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Set the screen size.
    m_device->setSize(size());

    // Create a painter from the device and render.
    QPainter painter(m_device);
    render(&painter);
}

void OpenGLWindow::renderLater()
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    //requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    // Render now if there is a window update.
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    // If the window is set visible then render.
    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    // If the window is not visible then there is no point in rendering.
    if (!isExposed())
        return;

    bool needsInitialize = false;

    // Create an OpenGL context if there is none.
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    // Enable the context
    m_context->makeCurrent(this);

    // Initialize OpenGL if needed.
    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    // Render.
    render();

    // Swap the double buffers.
    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}
