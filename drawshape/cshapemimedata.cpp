/*
 * Copyright (C) 2019 ~ %YEAR% Deepin Technology Co., Ltd.
 *
 * Author:     WangXing
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
#include "cshapemimedata.h"
#include "cgraphicsitem.h"

CShapeMimeData::CShapeMimeData(QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem *item, items ) {
        CGraphicsItem *copy = static_cast<CGraphicsItem *>(item)->duplicate();
        if (copy) {
            m_itemList.append(copy);
        }
    }
}
CShapeMimeData::~CShapeMimeData()
{
    foreach (QGraphicsItem *item, m_itemList ) {
        delete item;
        item = nullptr;
    }
    m_itemList.clear();
}

QList<CGraphicsItem *> CShapeMimeData::itemList() const
{
    return m_itemList;
}


