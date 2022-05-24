#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <ImageDefs.h>
#include <DDSPreviewerWidget.h>

#include <ApplicationSettings.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->processedAssetsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->processingProgressBar->setVisible(false);

    gApplicationHandlersManagerInstance.RegisterAssetProcessingNotificationsHandler(this);
    gApplicationHandlersManagerInstance.RegisterAssetProcessingProgressHandler(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreatePreviewWidget(DDSTexture &texture, const char* filePath)
{
    DDSPreviewerWidget* widget = new DDSPreviewerWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->SetTexture(texture);
    widget->setWindowTitle("DDS Previewer: " + QString(filePath));
    widget->show();
}

void MainWindow::OnAssetProcessed(const char* assetPath, const char* compressionType)
{
    QTableWidgetItem* pItemAssetPath = new QTableWidgetItem(assetPath);
    QTableWidgetItem* pItemCompressionType = new QTableWidgetItem(compressionType);

    ui->processedAssetsTable->insertRow(this->ui->processedAssetsTable->rowCount());

    ui->processedAssetsTable->setItem(this->ui->processedAssetsTable->rowCount() - 1, 0, pItemAssetPath);
    ui->processedAssetsTable->setItem(this->ui->processedAssetsTable->rowCount() - 1, 1, pItemCompressionType);
}

void MainWindow::OnStartProgress(int totalValue)
{
    ui->processingProgressBar->setValue(0);
    ui->processingProgressBar->setMaximum(totalValue);
    ui->processingProgressBar->setVisible(true);

    EnableControls(false);
}

void MainWindow::OnChangeCurrentAsset(const char *assetName)
{
    ui->currentAssetLabel->setText(QString::asprintf("Processing: %s", assetName));
}

void MainWindow::OnUpdateProgress()
{
    ui->processingProgressBar->setValue(ui->processingProgressBar->value() + 1);
}

void MainWindow::OnFinishProgress()
{
    ui->processingProgressBar->setValue(0);
    ui->processingProgressBar->setVisible(false);

    ui->currentAssetLabel->setText("");

    EnableControls(true);
}

void MainWindow::EnableControls(bool bEnable)
{
    ui->convertFolderButton->setEnabled(bEnable);
    ui->convertImageButton->setEnabled(bEnable);
    ui->processedAssetsTable->setEnabled(bEnable);
    ui->conversionModeComboBox->setEnabled(bEnable);
    ui->textureOrientationComboBox->setEnabled(bEnable);
}

void MainWindow::on_conversionModeComboBox_currentIndexChanged(int index)
{
    gApplicationSettingsInstance.SetConversionMode(static_cast<ConversionMode>(index));
}

void MainWindow::on_textureOrientationComboBox_currentIndexChanged(int index)
{
    gApplicationSettingsInstance.SetTextureOrientation(static_cast<GraphicsAPIType>(index));
}

void MainWindow::on_processedAssetsTable_cellDoubleClicked(int row, int column)
{
    if (column == 0) // only handle if double clicked on asset path column
    {
        QTableWidgetItem* pItem = ui->processedAssetsTable->item(row, column);
        DDSTexture texture = m_conversionController.ReadDDSTexture(pItem->text().toStdString().c_str());

        CreatePreviewWidget(texture, pItem->text().toStdString().c_str());
    }
}

void MainWindow::on_convertImageButton_clicked()
{
    QString inputImageName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    if(!inputImageName.isEmpty()&& !inputImageName.isNull())
    {
        QString outputTextureName = QFileDialog::getSaveFileName(this, tr("Save Texture"), "", tr("Texture File (*.dds)"));

        if(!outputTextureName.isEmpty()&& !outputTextureName.isNull())
        {
            m_conversionController.ConvertSingleImage(inputImageName.toStdString().c_str(), outputTextureName.toStdString().c_str());
        }

    }
}

void MainWindow::on_convertFolderButton_clicked()
{
    QDir inputFolder = QFileDialog::getExistingDirectory(this, tr("Open Folder"), "");

    if(!inputFolder.entryList().isEmpty())
    {
        QString outputFolder = QFileDialog::getExistingDirectory(this, tr("Save Folder"), "");

        if(!outputFolder.isEmpty()&& !outputFolder.isNull())
        {
            QStringList images = inputFolder.entryList(QStringList() << "*.jpg" << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tif" << "*.tiff", QDir::Files);

            QVector<QPair<QString, QString>> assetInfo;

            for (const auto& image : images)
            {
                QStringList pieces = image.split(".");
                QString fileName = pieces.value(0);
                QString outputFileName = outputFolder + "/" + fileName + ".dds";

                assetInfo.push_back({inputFolder.absoluteFilePath(image), outputFileName});
            }

            m_conversionController.ConvertMultipleImages(assetInfo);
        }
    }
}
