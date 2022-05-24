#ifndef ASSETPROCESSINGPROGRESSHANDLER_H
#define ASSETPROCESSINGPROGRESSHANDLER_H

class AssetProcessingProgressHandler
{
public:
    AssetProcessingProgressHandler() = default;

    virtual void OnStartProgress(int totalValue) = 0;
    virtual void OnChangeCurrentAsset(const char* assetName) = 0;
    virtual void OnUpdateProgress() = 0;
    virtual void OnFinishProgress() = 0;
};

#endif // ASSETPROCESSINGPROGRESSHANDLER_H
