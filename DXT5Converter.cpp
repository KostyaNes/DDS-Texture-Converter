#include "DXT5Converter.h"

#include <ImageFunc.h>

void DXT5Converter::ConvertTexel(const Texel &texel)
{
    std::pair <QColor, QColor> mainColors = FindMainColors(texel);

    QColor colors[4];
    colors[0] = mainColors.first;
    colors[1] = mainColors.second;
    colors[2] = QColor((colors[0].red()*2 + colors[1].red())/3, (colors[0].green()*2
                + colors[1].green())/3, (colors[0].blue()*2 + colors[1].blue())/3);
    colors[3] = QColor((colors[0].red() + colors[1].red()*2)/3, (colors[0].green()
                + colors[1].green()*2)/3, (colors[0].blue() + colors[1].blue()*2)/3);

    unsigned long long alpha = CalculateAlphaIndices(texel);
    unsigned int indexes =  CalculateColorIndices(texel, colors);

    unsigned short int C0 = RGB888ToRGB565(mainColors.first.rgb());
    unsigned short int C1 = RGB888ToRGB565(mainColors.second.rgb());

    DXT5ConvertedTexel convertedTexel;
    convertedTexel.alphaIndices = alpha;
    convertedTexel.C0 = C0;
    convertedTexel.C1 = C1;
    convertedTexel.indexes = indexes;
    m_convertedTexels.push_back(convertedTexel);
}

void DXT5Converter::SaveResult(QVector<char>& textureData)
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

unsigned long long DXT5Converter::CalculateAlphaIndices(const Texel &texel)
{
    std::pair <int, int> mainAlpha = GetMainAlpha(texel);
    QVector <int> alpha{};
    alpha.resize(8);

    if (mainAlpha.first == mainAlpha.second)
    {
        alpha[0] = mainAlpha.first;
        alpha[1] = mainAlpha.second;
        alpha[2] = (4 * alpha[0] + 1 * alpha[1] + 2) / 5;
        alpha[3] = (3 * alpha[0] + 2 * alpha[1] + 2) / 5;
        alpha[4] = (2 * alpha[0] + 3 * alpha[1] + 2) / 5;
        alpha[5] = (1 * alpha[0] + 4 * alpha[1] + 2) / 5;
        alpha[6] = 0;
        alpha[7] = 255;
    }
    else
    {
        if (mainAlpha.first < mainAlpha.second)
        {
            alpha[0] = mainAlpha.second;
            alpha[1] = mainAlpha.first;
        }
        else
        {
            alpha[0] = mainAlpha.first;
            alpha[1] = mainAlpha.second;
        }
        alpha[2] = (6 * alpha[0] + 1 * alpha[1] + 3) / 7;
        alpha[3] = (5 * alpha[0] + 2 * alpha[1] + 3) / 7;
        alpha[4] = (4 * alpha[0] + 3 * alpha[1] + 3) / 7;
        alpha[5] = (3 * alpha[0] + 4 * alpha[1] + 3) / 7;
        alpha[6] = (2 * alpha[0] + 5 * alpha[1] + 3) / 7;
        alpha[7] = (1 * alpha[0] + 6 * alpha[1] + 3) / 7;
    }
    unsigned long long value = 0;
    for (int i = 15; i >= 0; i--)
    {
        int min = abs(texel.pixels[i].alpha() - alpha[0]);
        int index = 0;
        for (int j = 1; j < 8; j++)
        {
            if (abs(texel.pixels[i].alpha() - alpha[j]) < min)
            {
                min = abs(texel.pixels[i].alpha() - alpha[j]);
                index = j;
            }
        }
        value = value << 3;
        value += static_cast<unsigned long long>(index);
    }
    value = value << 8;
    value += static_cast<unsigned long long>(alpha[1]);
    value = value << 8;
    value += static_cast<unsigned long long>(alpha[0]);
    return value;
}

std::pair<int, int> DXT5Converter::GetMainAlpha(const Texel &texel)
{
    int max = 0;
    int alpha0 = texel.pixels[0].alpha();
    int alpha1 = texel.pixels[0].alpha();
    for (int i = 0; i < 16; i++)
    {
        for (int j = i; j < 16; j++)
        {
            if (std::abs(texel.pixels[i].alpha() - texel.pixels[j].alpha()) > max)
            {
                alpha0 = texel.pixels[i].alpha();
                alpha1 = texel.pixels[j].alpha();
                max = abs(texel.pixels[i].alpha() - texel.pixels[j].alpha());
            }
        }
    }
    return std::pair <int, int> (alpha0, alpha1);
}
