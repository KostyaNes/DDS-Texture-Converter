#ifndef GENERICCONVERTER_H
#define GENERICCONVERTER_H

#include <ImageDefs.h>

#include <sstream>

#include <QColor>
#include <QVector>
#include <QImage>

class GenericConverter
{
public:
    GenericConverter(QImage& inputImage)
        : m_inputImage { inputImage }
    {
    }

public:
    void ConvertImage(QVector<char>& textureData);
    void ConvertImageInThread(QVector<char>& textureData);

protected:
    virtual void ConvertTexel(const Texel& texel) = 0;
    virtual void SaveResult(QVector<char>& textureData) = 0;

private:
    void SplitImageIntoTexels();

private:
    QImage& m_inputImage;
    QVector<Texel> m_texels;
};

#endif // GENERICCONVERTER_H
