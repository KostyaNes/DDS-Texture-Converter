#ifndef DDSPREVIEWERWIDGET_H
#define DDSPREVIEWERWIDGET_H

#include <QWidget>

#include <ImageDefs.h>
#include <OpenGLWidget.h>

namespace Ui {
class DDSPreviewerWidget;
}

class DDSPreviewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DDSPreviewerWidget(QWidget *parent = nullptr);
    ~DDSPreviewerWidget();

    void SetTexture(DDSTexture& texture);

private:
    void CreateOpenGLWidget();

private slots:
    void on_flipTextureButton_clicked();

private:
    Ui::DDSPreviewerWidget *ui;

    DDSTexture m_texture;
    bool m_bFlipTexture { false };
    OpenGLWidget* m_pOpenGLWidget { nullptr };
};

#endif // DDSPREVIEWERWIDGET_H
