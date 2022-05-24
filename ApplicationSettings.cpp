#include "ApplicationSettings.h"


ConversionMode ApplicationSettings::GetConversionMode() const
{
    return m_conversionMode;
}

void ApplicationSettings::SetConversionMode(const ConversionMode &conversionMode)
{
    m_conversionMode = conversionMode;
}

GraphicsAPIType ApplicationSettings::GetTextureOrientation() const
{
    return m_textureOrientation;
}

void ApplicationSettings::SetTextureOrientation(const GraphicsAPIType &textureOrientation)
{
    m_textureOrientation = textureOrientation;
}
