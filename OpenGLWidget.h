#pragma once

#include <QtWidgets>
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>
#include <ImageDefs.h>

struct Vertex {
   QVector3D coord;
   QVector2D texCoord;
   Vertex(qreal x, qreal y, qreal z, qreal s, qreal t):
     coord(x, y, z), texCoord(s, t)
   { }
};

class OpenGLWidget: public QOpenGLWidget, public QOpenGLFunctions_4_0_Core {
  private:

    DDSTexture m_texture;
    bool m_bFlipTexture { false };

    QOpenGLShaderProgram _qGLSProg;
    QOpenGLBuffer _qGLBufArray;
    QOpenGLBuffer _qGLBufIndex;
    QOpenGLTexture *_pQGLTex;

  public:
    explicit OpenGLWidget(QWidget* pParent):
      QOpenGLWidget(pParent),
      _qGLBufArray(QOpenGLBuffer::VertexBuffer),
      _qGLBufIndex(QOpenGLBuffer::IndexBuffer),
      _pQGLTex(nullptr)
    { }

    virtual ~OpenGLWidget()
    {
      makeCurrent();
      delete _pQGLTex;
      _qGLBufArray.destroy();
      _qGLBufIndex.destroy();
      doneCurrent();
    }

    // disabled: (to prevent accidental usage)
    OpenGLWidget(const OpenGLWidget&) = delete;
    OpenGLWidget& operator=(const OpenGLWidget&) = delete;

    void SetTexture(DDSTexture texture);

    void FlipTexture(bool bFlipTexture);

  protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;

  private:
    void initShaders();
    void initGeometry();

    void initTexture();
};
