#ifndef APPLICATIONHANDLERSMANAGER_H
#define APPLICATIONHANDLERSMANAGER_H

class AssetProcessingNotificationsHandler;
class AssetProcessingProgressHandler;

class ApplicationHandlersManager
{
public:
    ApplicationHandlersManager(ApplicationHandlersManager const&) = delete;
    void operator=(ApplicationHandlersManager const&) = delete;

    static ApplicationHandlersManager& GetInstance()
    {
        static ApplicationHandlersManager instance;
        return instance;
    }

    void RegisterAssetProcessingNotificationsHandler(AssetProcessingNotificationsHandler* pHandler);
    AssetProcessingNotificationsHandler* GetAssetProcessingNotificationsHandler();

    void RegisterAssetProcessingProgressHandler(AssetProcessingProgressHandler* pHandler);
    AssetProcessingProgressHandler* GetAssetProcessingProgressHandler();

private:
    ApplicationHandlersManager() {}

private:
    AssetProcessingNotificationsHandler* m_pAssetProcessingNotificationsHandler { nullptr };
    AssetProcessingProgressHandler* m_pAssetProcessingProgressHandler { nullptr };
};

#define gApplicationHandlersManagerInstance ApplicationHandlersManager::GetInstance()


#endif // APPLICATIONHANDLERSMANAGER_H
