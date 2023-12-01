#include "SpecialEffects.h"
#include <QPainter>
#include <QPaintEvent>

SpecialEffects::SpecialEffects(const QString &text, QWidget *parent)
    : QLabel(parent)
    , m_text(text)
{
   // setAttribute(Qt::WA_StyledBackground);      //启用样式背景绘制
    setAttribute(Qt::WA_TranslucentBackground); //背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAlignment(Qt::AlignCenter);
    setStyleSheet("QLabel{font-size:90px;}");
     // setStyleSheet("QLabel{font-size:90px;background-color:rgb(0,0,0);}");
}

SpecialEffects::~SpecialEffects()
{

}
void SpecialEffects::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void SpecialEffects::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 128));
    QPen pen;
    pen.setColor(m_color);
    painter.setPen(pen);
    painter.drawText(rect(), Qt::AlignCenter, m_text);

}
