#include <QPainter>
#include <QBoxLayout>
#include <QGraphicsDropShadowEffect>

#include "boxlayoutwidget.h"

BoxLayoutWidget::BoxLayoutWidget(QWidget *parent) : QWidget(parent), m_color(Qt::white)
{
    init();
}

BoxLayoutWidget::BoxLayoutWidget(QColor c, QWidget *parent) : QWidget(parent), m_color(c)
{
    init();
}

void BoxLayoutWidget::setColor(QColor c)
{
    m_color = c;
}

void BoxLayoutWidget::addWidget(QWidget *w)
{
    m_layout->addWidget(w);
}

void BoxLayoutWidget::resetLayout(QBoxLayout *l)
{
    if (nullptr == l) {
        return;
    }

    if (m_layout != nullptr && m_layout->count() > 0) {
        for (int i = m_layout->count(); i >= 0; i--) {
            l->addItem(m_layout->takeAt(i));
        }
    }

    m_layout = l;
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
}

void BoxLayoutWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen(m_color);
    painter.setPen(pen);
    QBrush brush(m_color);
    painter.setBrush(brush);
    painter.drawRoundRect(rect(), 10, 10);
}

void BoxLayoutWidget::init()
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Widget | Qt::NoDropShadowWindowHint);
    resetLayout(new QHBoxLayout);
}