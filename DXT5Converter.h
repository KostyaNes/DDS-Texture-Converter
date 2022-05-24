#ifndef DXT5CONVERTER_H
#define DXT5CONVERTER_H

#include "GenericConverter.h"

struct DXT5ConvertedTexel
{
    unsigned long long alphaIndices;
    unsigned short int C0;
    unsigned short int C1;
    unsigned int indexes;

};

class DXT5Converter
        : public GenericConverter
{
public:
    DXT5Converter(QImage& inputImage)
        : GenericConverter(inputImage)
    {
    }

protected:
    virtual void ConvertTexel(const Texel& texel);
    virtual void SaveResult(QVector<char>& textureData);

private:
    unsigned long long CalculateAlphaIndices(const Texel& texel);
    std::pair <int, int> GetMainAlpha(const Texel& texel);

private:
    QVector<DXT5ConvertedTexel> m_convertedTexels;
};

#endif // DXT5CONVERTER_H
