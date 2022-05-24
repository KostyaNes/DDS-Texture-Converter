#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include <ConversionController.h>
#include <ApplicationHandlersManager.h>
#include <AssetProcessingNotificationsHandler.h>
#include <AssetProcessingProgressHandler.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow
        : public QMainWindow
        , public AssetProcessingNotificationsHandler
        , public AssetProcessingProgressHandler
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void CreatePreviewWidget(DDSTexture& texture, const char* filePath);

    virtual void OnAssetProcessed(const char* assetPath, const char* compressionType) override;

    virtual void OnStartProgress(int totalValue) override;
    virtual void OnChangeCurrentAsset(const char* assetName) override;
    virtual void OnUpdateProgress() override;
    virtual void OnFinishProgress() override;

    virtual void EnableControls(bool bEnable);

private slots:
    void on_conversionModeComboBox_currentIndexChanged(int index);

    void on_textureOrientationComboBox_currentIndexChanged(int index);

    void on_processedAssetsTable_cellDoubleClicked(int row, int column);

    void on_convertImageButton_clicked();

    void on_convertFolderButton_clicked();

private:
    Ui::MainWindow *ui;

    ConversionController m_conversionController;
};
#endif // MAINWINDOW_H
