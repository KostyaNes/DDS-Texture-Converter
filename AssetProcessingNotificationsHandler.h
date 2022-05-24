#ifndef ASSETPROCESSINGNOTIFICATIONHANDLER_H
#define ASSETPROCESSINGNOTIFICATIONHANDLER_H


class AssetProcessingNotificationsHandler
{
public:
    AssetProcessingNotificationsHandler() = default;

    virtual void OnAssetProcessed(const char* assetPath, const char* compressionType) = 0;
};

#endif // ASSETPROCESSINGNOTIFICATIONHANDLER_H
