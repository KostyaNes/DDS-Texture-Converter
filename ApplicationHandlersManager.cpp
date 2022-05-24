#include "ApplicationHandlersManager.h"

#include <AssetProcessingNotificationsHandler.h>
#include <AssetProcessingProgressHandler.h>

void ApplicationHandlersManager::RegisterAssetProcessingNotificationsHandler(AssetProcessingNotificationsHandler* pHandler)
{
    m_pAssetProcessingNotificationsHandler = pHandler;
}

AssetProcessingNotificationsHandler* ApplicationHandlersManager::GetAssetProcessingNotificationsHandler()
{
    return m_pAssetProcessingNotificationsHandler;
}

void ApplicationHandlersManager::RegisterAssetProcessingProgressHandler(AssetProcessingProgressHandler *pHandler)
{
    m_pAssetProcessingProgressHandler = pHandler;
}

AssetProcessingProgressHandler *ApplicationHandlersManager::GetAssetProcessingProgressHandler()
{
    return m_pAssetProcessingProgressHandler;
}
