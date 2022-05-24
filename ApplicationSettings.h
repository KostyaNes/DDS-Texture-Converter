#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <ImageDefs.h>

class ApplicationSettings
{
public:
    ApplicationSettings(ApplicationSettings const&) = delete;
    void operator=(ApplicationSettings const&) = delete;

    static ApplicationSettings& GetInstance()
    {
        static ApplicationSettings instance;
        return instance;
    }

    ConversionMode GetConversionMode() const;
    void SetConversionMode(const ConversionMode& conversionMode);

    GraphicsAPIType GetTextureOrientation() const;
    void SetTextureOrientation(const GraphicsAPIType& textureOrientation);

private:
    ApplicationSettings() {}

private:
    ConversionMode m_conversionMode;
    GraphicsAPIType m_textureOrientation;
};

#define gApplicationSettingsInstance ApplicationSettings::GetInstance()

#endif // APPLICATIONSETTINGS_H
