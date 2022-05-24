#pragma once

#include <ImageDefs.h>

#include <cmath>

#include <QColor>
#include <QDebug>

inline unsigned short int RGB888ToRGB565(QRgb aPixel)
{
    int red   = ( aPixel >> 16) & 0xFF;
    int green = ( aPixel >> 8 ) & 0xFF;
    int blue  =   aPixel        & 0xFF;

    unsigned short  B =  (blue  >> 3)        & 0x001F;
    unsigned short  G = ((green >> 2) <<  5) & 0x07E0;
    unsigned short  R = ((red   >> 3) << 11) & 0xF800;

    return static_cast<unsigned short int> (R | G | B);
}

inline unsigned short int RGB888ToRGB565(QColor pixel)
{
    int red   = pixel.red();
    int green = pixel.green();
    int blue  = pixel.blue();

    unsigned short  B =  (blue  >> 3);
    unsigned short  G = ((green >> 2) <<  5);
    unsigned short  R = ((red   >> 3) << 11);

    return static_cast<unsigned short int> (R | G | B);
}

inline QColor RGB565ToRGB888(unsigned short int color)
{
    int r5 = (color & 0xf800) >> 11;
    int g6 = (color & 0x07e0) >> 5;
    int b5 = color & 0x001f;

    int r8 = ( r5 << 3 ) | (r5 >> 2);
    int g8 = ( g6 << 2 ) | (g6 >> 4);
    int b8 = ( b5 << 3 ) | (b5 >> 2);

    return QColor(r8, g8, b8);
}

inline double GetColorDistance(QColor color1, QColor color2)
{
    return std::sqrt(  (color1.red()   -   color2.red())
                     * (color1.red()   -   color2.red())
                     + (color1.green() -   color2.green())
                     * (color1.green() -   color2.green())
                     + (color1.blue()  -   color2.blue())
                     * (color1.blue()  -   color2.blue()));
}

inline double GetColorDistanceFromRGB565(QColor source, QColor target)
{
    // Double conversion in order to get more accurate result
    target = RGB565ToRGB888(RGB888ToRGB565(target.rgb()));

    return std::sqrt(  (source.red()   -   target.red())
                     * (source.red()   -   target.red())
                     + (source.green() -   target.green())
                     * (source.green() -   target.green())
                     + (source.blue()  -   target.blue())
                     * (source.blue()  -   target.blue()));
}

inline std::pair <QColor, QColor> FindMainColors(const Texel& texel)
{
    double max = 0;
    QColor C0 = texel.pixels[0];
    QColor C1 = texel.pixels[0];
    for (int i = 0; i < 16; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            if (i != j)
            {
                double res = GetColorDistance(texel.pixels[i], texel.pixels[j]);
                if (res > max)
                {
                    max = res;
                    C0 = texel.pixels[i];
                    C1 = texel.pixels[j];
                }
            }
        }
    }
    return std::pair<QColor, QColor> (C0, C1);
}

inline unsigned int CalculateColorIndices(const Texel& texel, const QColor (&color)[4])
{
    unsigned int value = 0;
    for (int i = 15; i >= 0; i--)
    {
        double res[4];
        int min = 0;
        if (texel.pixels[i].alpha() == 0)
        {
            min = 3;
        }
        else
        {
            res[0] = GetColorDistance(texel.pixels[i], color[0]);

            res[1] = GetColorDistance(texel.pixels[i], color[1]);
            if (res[min] > res[1])
            {
                min = 1;
            }

            res[2] = GetColorDistance(texel.pixels[i], color[2]);
            if (res[min] > res[2])
            {
                min = 2;
            }

            res[3] = GetColorDistance(texel.pixels[i], color[3]);
            if (res[min] > res[3])
            {
                min = 3;
            }
        }
        value = value << 2;
        value += static_cast<unsigned int>(min);
    }
    return value;
}

inline bool CheckZeroAlpha(const Texel& texel)
{
    for (int i = 0; i < 16; i++)
    {
        if (texel.pixels[i].alpha() == 0)
            return true;
    }
    return false;
}
