#include "DDSPreviewerWidget.h"
#include "ui_DDSPreviewerWidget.h"

#include <math.h>

namespace WidgetSizeDefs
{
    const int maxW = 1280;
    const int maxH = 720;
    const int minW = 400;
    const int minH = 200;

    const int xOffset = 50;
    const int yOffset = 50;
}

DDSPreviewerWidget::DDSPreviewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDSPreviewerWidget)
{
    ui->setupUi(this);
}

DDSPreviewerWidget::~DDSPreviewerWidget()
{
    delete m_pOpenGLWidget;

    delete ui;
}

void DDSPreviewerWidget::SetTexture(DDSTexture &texture)
{
    m_texture = texture;

    CreateOpenGLWidget();
}

void DDSPreviewerWidget::CreateOpenGLWidget()
{
    if (m_pOpenGLWidget)
    {
        delete m_pOpenGLWidget;
    }

    m_pOpenGLWidget = new OpenGLWidget(this);

    int widgetH = m_texture.m_height;
    int widgetW = m_texture.m_width;

    if (widgetH > WidgetSizeDefs::maxH)
    {
        float multiplier = widgetH / WidgetSizeDefs::maxH;
        widgetH = WidgetSizeDefs::maxH;
        widgetW = round(widgetW / multiplier);
    }

    if (widgetW > WidgetSizeDefs::maxW)
    {
        float multiplier = widgetW / WidgetSizeDefs::maxW;
        widgetW = WidgetSizeDefs::maxW;
        widgetH = round(widgetH / multiplier);
    }

    if (widgetH < WidgetSizeDefs::minH)
    {
        float multiplier = WidgetSizeDefs::minH / widgetH;
        widgetH = WidgetSizeDefs::minH;
        widgetW = round(widgetW * multiplier);
    }

    if (widgetW < WidgetSizeDefs::minW)
    {
        float multiplier = WidgetSizeDefs::minW / widgetW;
        widgetW = WidgetSizeDefs::minW;
        widgetH = round(widgetH * multiplier);
    }

    this->resize(widgetW + WidgetSizeDefs::xOffset * 2,  widgetH + WidgetSizeDefs::yOffset * 2);

    m_pOpenGLWidget->resize(widgetW,  widgetH);
    m_pOpenGLWidget->move(WidgetSizeDefs::xOffset, WidgetSizeDefs::yOffset);
    m_pOpenGLWidget->SetTexture(m_texture);
    m_pOpenGLWidget->FlipTexture(m_bFlipTexture);

    m_pOpenGLWidget->show();
}

void DDSPreviewerWidget::on_flipTextureButton_clicked()
{
    m_bFlipTexture = m_bFlipTexture ? false : true;

    CreateOpenGLWidget();
}
