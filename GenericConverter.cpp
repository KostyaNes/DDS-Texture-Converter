#include "GenericConverter.h"

#include <ApplicationSettings.h>
#include <ApplicationHandlersManager.h>
#include <AssetProcessingProgressHandler.h>

#include <thread>

void GenericConverter::SplitImageIntoTexels()
{
    if (gApplicationSettingsInstance.GetTextureOrientation() == GraphicsAPIType::DirectX)
    {
        for (int i = 0; i < m_inputImage.height(); i+=4)
        {
            for (int j = 0; j < m_inputImage.width(); j+=4)
            {
                m_texels.push_back(Texel());
                int pixelCount = 0;
                for (int y = i; y < i + 4; y++)
                {
                    for (int x = j; x < j + 4; x++)
                    {
                        if (y >= m_inputImage.height() || x >= m_inputImage.width())
                        {
                            m_texels.last().pixels[pixelCount] = QColor(0, 0, 0, 0);
                        }
                        else
                        {
                            m_texels.last().pixels[pixelCount] = m_inputImage.pixelColor(x, y);
                        }
                        pixelCount++;
                    }
                }

            }
        }
    }
    else
    {
        for (int i = m_inputImage.height() - 1; i >= 0; i-=4)
        {
            for (int j = 0; j < m_inputImage.width(); j+=4)
            {
                m_texels.push_back(Texel());
                int pixelCount = 0;
                for (int y = i; y > i - 4; y--)
                {
                    for (int x = j; x < j + 4; x++)
                    {
                        if (y < 0 || x >= m_inputImage.width())
                        {
                            m_texels.last().pixels[pixelCount] = QColor(0, 0, 0, 0);
                        }
                        else
                        {
                            m_texels.last().pixels[pixelCount] = m_inputImage.pixelColor(x, y);
                        }
                        pixelCount++;
                    }
                }

            }
        }
    }
}

void GenericConverter::ConvertImage(QVector<char>& textureData)
{
    SplitImageIntoTexels();
    for (const auto& i : m_texels)
    {
        ConvertTexel(i);
        gApplicationHandlersManagerInstance.GetAssetProcessingProgressHandler()->OnUpdateProgress();
    }

    SaveResult(textureData);
}

void GenericConverter::ConvertImageInThread(QVector<char>& textureData)
{
    std::thread workingThread(&GenericConverter::ConvertImage, this, std::ref(textureData));

    workingThread.join();
}
