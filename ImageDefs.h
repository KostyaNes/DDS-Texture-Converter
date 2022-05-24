#ifndef IMAGEDEFS_H
#define IMAGEDEFS_H

#include <QColor>
#include <DDSDefs.h>

enum GraphicsAPIType
{
    DirectX,
    OpenGL
};

enum ConversionMode
{
    DXT1,
    DXT3,
    DXT5,
    Auto
};

struct Texel
{
    QColor pixels[16];
};

struct DDSTexture
{
    int m_width { 0 };
    int m_height { 0 };
    QVector<char> m_data;
    CompressionType m_format { CompressionType::dxt1 };

    DDSTexture() = default;

    DDSTexture(int w, int h, QVector<char>& data, CompressionType format)
    {
        m_width = w;
        m_height = h;
        m_data = data;
        m_format = format;
    }
};

#endif // IMAGEDEFS_H
