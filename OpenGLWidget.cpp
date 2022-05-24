#include "OpenGLWidget.h"

#include <QtWidgets>
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>
#include <ImageDefs.h>

namespace Shaders
{
    const char *vertexShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform mat4 mvp_matrix;\n"
      "\n"
      "attribute vec4 a_position;\n"
      "attribute vec2 a_texcoord;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Calculate vertex position in screen space\n"
      "  gl_Position = mvp_matrix * a_position;\n"
      "\n"
      "  // Pass texture coordinate to fragment shader\n"
      "  // Value will be automatically interpolated to fragments inside polygon faces\n"
      "  v_texcoord = a_texcoord;\n"
      "}\n";

    const char *fragmentShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform sampler2D texture;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Set fragment color from texture\n"
      "#if 0 // test check tex coords\n"
      "  gl_FragColor = vec4(1, v_texcoord.x, v_texcoord.y, 1);\n"
      "#else // (not) 0;\n"
      "  vec4 color = texture2D(texture, vec2(v_texcoord.x, 1 - v_texcoord.y));\n"
      "  //if (color.a < 0.01) discard;\n"
      "  gl_FragColor = color;\n"
      "#endif // 0\n"
      "}\n";

    const char *flippedFragmentShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform sampler2D texture;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Set fragment color from texture\n"
      "#if 0 // test check tex coords\n"
      "  gl_FragColor = vec4(1, v_texcoord.x, v_texcoord.y, 1);\n"
      "#else // (not) 0;\n"
      "  vec4 color = texture2D(texture, vec2(v_texcoord.x, v_texcoord.y));\n"
      "  //if (color.a < 0.01) discard;\n"
      "  gl_FragColor = color;\n"
      "#endif // 0\n"
      "}\n";
}

void OpenGLWidget::SetTexture(DDSTexture texture)
{
    m_texture = texture;
}

void OpenGLWidget::FlipTexture(bool bFlipTexture)
{
    m_bFlipTexture = bFlipTexture;
    hide();
    show();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.5, 0.5, 0.5, 0);

    initShaders();
    initGeometry();
    initTexture();
}
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _pQGLTex->bind();
    QMatrix4x4 mat; mat.setToIdentity();
    _qGLSProg.setUniformValue("mvp_matrix", mat);
    _qGLSProg.setUniformValue("texture", 0);
    // draw geometry
    _qGLBufArray.bind();
    _qGLBufIndex.bind();
    quintptr offset = 0;
    int coordLocation = _qGLSProg.attributeLocation("a_position");
    _qGLSProg.enableAttributeArray(coordLocation);
    _qGLSProg.setAttributeBuffer(coordLocation, GL_FLOAT, offset, 3, sizeof(Vertex));
    offset += sizeof(QVector3D);
    int texCoordLocation = _qGLSProg.attributeLocation("a_texcoord");
    _qGLSProg.enableAttributeArray(texCoordLocation);
    _qGLSProg.setAttributeBuffer(texCoordLocation, GL_FLOAT, offset, 2, sizeof(Vertex));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
}

void OpenGLWidget::initShaders()
{
    if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Vertex, QString::fromLatin1(Shaders::vertexShader)))
    {
      parentWidget()->hide();
    }

    if (m_bFlipTexture)
    {
      if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Fragment, QString::fromLatin1(Shaders::flippedFragmentShader)))
      {
          parentWidget()->hide();
      }
    }
    else
    {
      if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Fragment, QString::fromLatin1(Shaders::fragmentShader)))
      {
          parentWidget()->hide();
      }
    }

    if (!_qGLSProg.link()) parentWidget()->hide();
    if (!_qGLSProg.bind()) parentWidget()->hide();
}

void OpenGLWidget::initGeometry()
{
    Vertex vertices[] = {
        //  x      y      z     s     t
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
        { +1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
        { +1.0f, +1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f, +1.0f, 0.0f, 0.0f, 1.0f }
    };
    enum { nVtcs = sizeof vertices / sizeof *vertices };
    // OpenGL ES doesn't have QUAD. A TRIANGLE_STRIP does as well.
    GLushort indices[] = { 3, 0, 2, 1 };
    //GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    enum { nIdcs = sizeof indices / sizeof *indices };
    _qGLBufArray.create();
    _qGLBufArray.bind();
    _qGLBufArray.allocate(vertices, nVtcs * sizeof (Vertex));
    _qGLBufIndex.create();
    _qGLBufIndex.bind();
    _qGLBufIndex.allocate(indices, nIdcs * sizeof (GLushort));
}

void OpenGLWidget::initTexture()
{
    _pQGLTex = new QOpenGLTexture(QOpenGLTexture::Target2D);

    switch (m_texture.m_format)
    {
      case CompressionType::dxt1:
        _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT1);
        _pQGLTex->setSize(m_texture.m_width, m_texture.m_height);
        _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16_RGB5A1);
        break;
      case CompressionType::dxt3:
        _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT3);
        _pQGLTex->setSize(m_texture.m_width, m_texture.m_height);
        _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);
        break;
      case CompressionType::dxt5:
        _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT5);
        _pQGLTex->setSize(m_texture.m_width, m_texture.m_height);
        _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);
        break;
    }

    _pQGLTex->setCompressedData((int)m_texture.m_data.size(), m_texture.m_data.data());

    _pQGLTex->setMinificationFilter(QOpenGLTexture::Nearest);
    _pQGLTex->setMagnificationFilter(QOpenGLTexture::Nearest);
    _pQGLTex->setWrapMode(QOpenGLTexture::ClampToEdge);
}
