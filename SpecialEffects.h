#ifndef SPECIALEFFECTS_H
#define SPECIALEFFECTS_H

#include <QLabel>
#include <QFont>
#include <QPen>

class SpecialEffects :public QLabel
{

public:
    SpecialEffects(const QString &text, QWidget *parent = nullptr);
    ~SpecialEffects();
    void setColor(const QColor &color);
    void paintEvent(QPaintEvent* event);

private:
    QString     m_text;
    QColor      m_color{ Qt::white };
};

#endif // TITLEQLABEL_H
