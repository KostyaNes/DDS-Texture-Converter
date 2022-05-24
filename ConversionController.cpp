#include "ConversionController.h"

#include <DXT1Converter.h>
#include <DXT3Converter.h>
#include <DXT5Converter.h>

#include <ApplicationSettings.h>
#include <ApplicationHandlersManager.h>
#include <AssetProcessingNotificationsHandler.h>
#include <AssetProcessingProgressHandler.h>

#include <memory>
#include <sstream>
#include <thread>

void ConversionController::ConvertSingleImage(const char* inputImagePath, const char* outputImagePath)
{
    gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnStartProgress(EvaluateImage(inputImagePath));
    gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnChangeCurrentAsset(inputImagePath);

    ConvertImage(inputImagePath, outputImagePath);

    gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnFinishProgress();
}

void ConversionController::ConvertMultipleImages(const QVector<QPair<QString, QString>> &assetInfo)
{
    int totalValue = 0;

    for (const auto& i : assetInfo)
    {
        totalValue += EvaluateImage(i.first.toStdString().c_str());
    }

    gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnStartProgress(totalValue);

    for (const auto& i : assetInfo)
    {
        gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnChangeCurrentAsset(i.first.toStdString().c_str());
        ConvertImage(i.first.toStdString().c_str(), i.second.toStdString().c_str());
    }

    gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnFinishProgress();
}

void ConversionController::ConvertImage(const char *inputImagePath, const char *outputImagePath)
{
    QImage image = QImage(inputImagePath);
    std::unique_ptr<GenericConverter> converter = nullptr;
    CompressionType compressionType = CompressionType::dxt1;

    switch(gApplicationSettingsInstance.GetConversionMode())
    {
        case ConversionMode::DXT1:
            //converter = std::make_unique<DXT1Converter>(image);
            converter = std::unique_ptr<DXT1Converter>(new DXT1Converter(image));
            compressionType = CompressionType::dxt1;
        break;
        case ConversionMode::DXT3:
            //converter = std::make_unique<DXT3Converter>(image);
            converter = std::unique_ptr<DXT3Converter>(new DXT3Converter(image));
            compressionType = CompressionType::dxt3;
        break;
        case ConversionMode::DXT5:
            //converter = std::make_unique<DXT5Converter>(image);
            converter = std::unique_ptr<DXT5Converter>(new DXT5Converter(image));
            compressionType = CompressionType::dxt5;
        break;
        case ConversionMode::Auto:
            //TODO: Auto Detection
        break;
    }

    QVector<char> textureData;
    if (converter)
    {
        DDSTexture texture;
        texture.m_width = image.width();
        texture.m_height = image.height();
        texture.m_format = compressionType;

        converter->ConvertImage(texture.m_data);
        //converter->ConvertImageInThread(texture.m_data);

        m_ddsWriter.WriteDDSTexture(texture, outputImagePath);

        const char* compressionTypeString = "Error";
        switch (compressionType)
        {
            case CompressionType::dxt1:
                compressionTypeString = "DXT1";
            break;
            case CompressionType::dxt3:
                compressionTypeString = "DXT3";
            break;
            case CompressionType::dxt5:
                compressionTypeString = "DXT5";
            break;
        }

        gApplicationHandlersManagerInstance.GetAssetProcessingNotificationsHandler()->OnAssetProcessed(outputImagePath, compressionTypeString);
    }
}

int ConversionController::EvaluateImage(const char *imagePath)
{
    QImage image = QImage(imagePath);
    return (image.width() + (4 - image.width() % 4) % 4) * (image.height() + (4 - image.height() % 4) % 4) / 16;
}

DDSTexture ConversionController::ReadDDSTexture(const char *filePath)
{
    return m_ddsReader.ReadDDSTexture(filePath);
}
