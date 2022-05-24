#ifndef DXT1CONVERTER_H
#define DXT1CONVERTER_H

#include "GenericConverter.h"

struct DXT1ConvertedTexel
{
    unsigned short int C0;
    unsigned short int C1;
    unsigned int indexes;

};

class DXT1Converter
        : public GenericConverter
{
public:
    DXT1Converter(QImage& inputImage)
        : GenericConverter(inputImage)
    {
    }

protected:
    virtual void ConvertTexel(const Texel& texel);
    virtual void SaveResult(QVector<char>& textureData);

private:
    QVector<DXT1ConvertedTexel> m_convertedTexels;
};

#endif // DXT1CONVERTER_H
