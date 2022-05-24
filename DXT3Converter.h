#ifndef DXT3CONVERTER_H
#define DXT3CONVERTER_H

#include "GenericConverter.h"


struct DXT3ConvertedTexel
{
    unsigned long long int alpha4Bit;
    unsigned short int C0;
    unsigned short int C1;
    unsigned int indexes;

};

class DXT3Converter
        : public GenericConverter
{
public:
    DXT3Converter(QImage& inputImage)
        : GenericConverter(inputImage)
    {
    }

protected:
    virtual void ConvertTexel(const Texel& texel);
    virtual void SaveResult(QVector<char>& textureData);

private:
    unsigned long long int Get4BitAlpha(const Texel& texel);

private:
    QVector<DXT3ConvertedTexel> m_convertedTexels;
};

#endif // DXT3CONVERTER_H
