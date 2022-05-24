#include "DXT3Converter.h"

#include <ImageFunc.h>

void DXT3Converter::ConvertTexel(const Texel &texel)
{
    std::pair <QColor, QColor> mainColors = FindMainColors(texel);
    QColor colors[4];
    colors[0] = mainColors.first;
    colors[1] = mainColors.second;
    colors[2] = QColor((colors[0].red()*2 + colors[1].red())/3, (colors[0].green()*2
                + colors[1].green())/3, (colors[0].blue()*2 + colors[1].blue())/3);
    colors[3] = QColor((colors[0].red() + colors[1].red()*2)/3, (colors[0].green()
                + colors[1].green()*2)/3, (colors[0].blue() + colors[1].blue()*2)/3);

    unsigned long long int alpha = Get4BitAlpha(texel);

    unsigned int indexes =  CalculateColorIndices(texel, colors);
    unsigned short int C0 = RGB888ToRGB565(mainColors.first.rgb());
    unsigned short int C1 = RGB888ToRGB565(mainColors.second.rgb());

    DXT3ConvertedTexel convertedTexel;
    convertedTexel.alpha4Bit = alpha;
    convertedTexel.C0 = C0;
    convertedTexel.C1 = C1;
    convertedTexel.indexes = indexes;
    m_convertedTexels.push_back(convertedTexel);
}

void DXT3Converter::SaveResult(QVector<char>& textureData)
{
    for (auto& convertedTexel : m_convertedTexels)
    {
        char bytes[16];
        std::copy(reinterpret_cast<char*>(&convertedTexel),
                  reinterpret_cast<char*>(&convertedTexel) + 16,
                  bytes);
        for (auto byte: bytes)
        {
            textureData.push_back(byte);
        }
    }
}

unsigned long long DXT3Converter::Get4BitAlpha(const Texel &texel)
{
    unsigned long long int value = 0;
    for (int i = 15; i >= 0; i--)
    {
        value = value << 4;
        int buffer = texel.pixels[i].alpha() >> 4;
        value += static_cast<unsigned long long int>(buffer);
    }
    return value;
}
