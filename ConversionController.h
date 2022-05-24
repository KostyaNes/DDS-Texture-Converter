#ifndef CONVERSIONCONTROLLER_H
#define CONVERSIONCONTROLLER_H

#include <DDSWriter.h>
#include <DDSReader.h>

#include <ImageDefs.h>

#include <memory>

#include <QImage>

class ConversionController
{
public:
    ConversionController() = default;

    void ConvertSingleImage(const char* inputImagePath, const char* outputImagePath);
    void ConvertMultipleImages(const QVector<QPair<QString, QString>>& assetInfo);
    DDSTexture ReadDDSTexture(const char* filePath);

private:
    void ConvertImage(const char* inputImagePath, const char* outputImagePath);
    int EvaluateImage(const char* imagePath);

private:
    DDSWriter m_ddsWriter;
    DDSReader m_ddsReader;
};

#endif // CONVERSIONCONTROLLER_H
