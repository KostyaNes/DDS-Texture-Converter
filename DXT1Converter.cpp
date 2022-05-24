#include "DXT1Converter.h"

#include <ImageFunc.h>

void DXT1Converter::ConvertTexel(const Texel &texel)
{
    std::pair <QColor, QColor> mainColors = FindMainColors(texel);
    QColor colors[4];
    colors[0] = mainColors.first;
    colors[1] = mainColors.second;
    unsigned short int C0 = RGB888ToRGB565(mainColors.first);
    unsigned short int C1 = RGB888ToRGB565(mainColors.second);

    // Special case for DXT1 algorithm when zero alpha is present
    if (CheckZeroAlpha(texel) || C0 == C1)
    {
        if (C0 > C1)
        {
            std::swap(C0, C1);
            colors[0] = mainColors.second;
            colors[1] = mainColors.first;
        }
        else
        {
            colors[0] = mainColors.first;
            colors[1] = mainColors.second;
        }
        colors[2] = QColor(colors[0].red()*1/2 + colors[1].red()*1/2, colors[0].green()*1/2
                           + colors[1].green()*1/2, colors[0].blue()*1/2 + colors[1].blue()*1/2);
        colors[3] = QColor(0, 0, 0);
    }
    else
    {
        if (C0 < C1)
        {
            std::swap(C0, C1);
            colors[0] = mainColors.second;
            colors[1] = mainColors.first;
        }
        else
        {
            colors[0] = mainColors.first;
            colors[1] = mainColors.second;
        }
        colors[2] = QColor(colors[0].red()*2/3 + colors[1].red()*1/3, colors[0].green()*2/3
                    + colors[1].green()*1/3, colors[0].blue()*2/3 + colors[1].blue()*1/3);
        colors[3] = QColor(colors[0].red()*1/3 + colors[1].red()*2/3, colors[0].green()*1/3
                    + colors[1].green()*2/3, colors[0].blue()*1/3 + colors[1].blue()*2/3);
    }

    unsigned int indexes =  CalculateColorIndices(texel, colors);
    DXT1ConvertedTexel convertedTexel;
    convertedTexel.C0 = C0;
    convertedTexel.C1 = C1;
    convertedTexel.indexes = indexes;
    m_convertedTexels.push_back(convertedTexel);
}

void DXT1Converter::SaveResult(QVector<char>& textureData)
{
    for (auto& convertedTexel : m_convertedTexels)
    {
        char bytes[8];
        std::copy(reinterpret_cast<char*>(&convertedTexel),
                  reinterpret_cast<char*>(&convertedTexel) + 8,
                  bytes);
        for (auto byte: bytes)
        {
            textureData.push_back(byte);
        }
    }
}
