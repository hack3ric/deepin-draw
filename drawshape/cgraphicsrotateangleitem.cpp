/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     Renran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "cgraphicsrotateangleitem.h"

#include <QPainter>

CGraphicsRotateAngleItem::CGraphicsRotateAngleItem(qreal rotateAngle, QGraphicsItem *parent)
    : QGraphicsRectItem (parent)
    , m_rotateAngle(rotateAngle)
{
    m_width = 45;
    m_height = 20;
    setRect(-m_width / 2, -m_height / 2, m_width, m_height);
}

CGraphicsRotateAngleItem::CGraphicsRotateAngleItem(const QRectF &rect, qreal rotateAngle, QGraphicsItem *parent)
    : QGraphicsRectItem (rect, parent)
    , m_rotateAngle(rotateAngle)

{
    setRect(rect);
}

void CGraphicsRotateAngleItem::updateRotateAngle(qreal rotateAngle)
{
    m_rotateAngle =  rotateAngle;
}

void CGraphicsRotateAngleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#ececf8"));

    painter->drawRoundRect(rect());

    QString angle = QString("%1°").arg(QString::number(m_rotateAngle, 'f', 1));
    painter->setPen(Qt::black);
    painter->drawText(rect(), angle);
}