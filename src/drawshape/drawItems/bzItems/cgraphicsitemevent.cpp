#include "cgraphicsitemevent.h"
#include <QDebug>

CGraphItemEvent::CGraphItemEvent(CGraphItemEvent::EItemType tp,
                                 const QPointF &oldPos,
                                 const QPointF &pos)
    : _tp(tp), _oldPos(oldPos), _pos(pos)
{

}

void CGraphItemEvent::setOldPos(const QPointF &pos)
{
    _oldPos = pos;
    _transDirty = true;
}

void CGraphItemEvent::setPos(const QPointF &pos)
{
    _pos = pos;
    _transDirty = true;
}

void CGraphItemEvent::setBeginPos(const QPointF &pos)
{
    _beginPos = pos;
    _transDirty = true;
}

void CGraphItemEvent::setCenterPos(const QPointF &pos)
{
    _centerPos = pos;
    _transDirty = true;
}

void CGraphItemEvent::setKeepOrgRadio(bool b)
{
    _isKeepOrgRadio = b;
}

void CGraphItemEvent::setOrgSize(const QSizeF &sz)
{
    _orgSz = sz;
    _transDirty = true;
}

void CGraphItemEvent::setEventPhase(EChangedPhase ph)
{
    _phase = ph;
}

//bool CGraphItemEvent::isXTransBlocked() const
//{
//    return _blockXTrans;
//}

void CGraphItemEvent::setXTransBlocked(bool b)
{
    _blockXTrans = b;
    _transDirty = true;
}

//bool CGraphItemEvent::isYTransBlocked() const
//{
//    return _blockYTrans;
//}

void CGraphItemEvent::setYTransBlocked(bool b)
{
    _blockYTrans = b;
    _transDirty = true;
}

//bool CGraphItemEvent::isXNegtiveOffset() const
//{
//    return _isXNegtiveOffset;
//}

void CGraphItemEvent::setXNegtiveOffset(bool b)
{
    _isXNegtiveOffset = b;
    _transDirty = true;
}

//bool CGraphItemEvent::isYNegtiveOffset() const
//{
//    return _isYNegtiveOffset;
//}

void CGraphItemEvent::setYNegtiveOffset(bool b)
{
    _isYNegtiveOffset = b;
    _transDirty = true;
}

void CGraphItemEvent::updateTrans()
{
    if (!_orgSz.isValid()) {
        qDebug() << "_orgSz = " << _orgSz << "_centerPos = " << _centerPos;
        return;
    }

    QTransform trans;

    const QPointF centerPos = this->centerPos();

    const qreal offsetX = (_isXNegtiveOffset ? -1 : 1) * this->offset().x();
    const qreal offsetY = (_isYNegtiveOffset ? -1 : 1) * this->offset().y();

    qreal sX = _blockXTrans ? 1.0 : (offsetX + _orgSz.width()) / _orgSz.width();
    qreal sY = _blockYTrans ? 1.0 : (offsetY + _orgSz.height()) / _orgSz.height();

    if (qFuzzyIsNull(sX) || sX < 0) {
        sX  = 1;
    }
    if (qFuzzyIsNull(sY) || sY < 0) {
        sY  = 1;
    }

    if (_isKeepOrgRadio) {
        if (qAbs(offsetY) > qAbs(offsetX)) {
            sX = sY;
        } else {
            sY = sX;
        }
    }

    trans.translate(centerPos.x(), centerPos.y());
    trans.scale(sX, sY);
    trans.translate(-centerPos.x(), -centerPos.y());

    _trans = trans;

    _transDirty = false;
}

QTransform CGraphItemEvent::trans()
{
    if (_transDirty) {
        updateTrans();
    }
    return _trans;
}

void CGraphItemEvent::setTrans(const QTransform &trans)
{
    _trans = trans;
    _transDirty = false;
}

CGraphItemEvent CGraphItemEvent::transToEvent(const QTransform &tran, const QSizeF &newOrgSz)
{
    CGraphItemEvent event(this->type());
    event.setEventPhase(this->eventPhase());

    event.setPos(tran.map(this->pos()));
    event.setOldPos(tran.map(this->oldPos()));
    event.setBeginPos(tran.map(this->beginPos()));
    event.setCenterPos(tran.map(this->centerPos()));
    event.setOrgSize(newOrgSz);

    event._blockXTrans = this->_blockXTrans;
    event._blockYTrans = this->_blockYTrans;
    event._isXNegtiveOffset = this->_isXNegtiveOffset;
    event._isYNegtiveOffset = this->_isYNegtiveOffset;
    event._isKeepOrgRadio   = this->_isKeepOrgRadio;

    event._pressedDirection = this->_pressedDirection;
    event._orgToolEventTp = this->_orgToolEventTp;

    event._oldScenePos = this->_oldScenePos;
    event._scenePos = this->_scenePos;
    event._sceneBeginPos = this->_sceneBeginPos;
    event._sceneCenterPos = this->_sceneCenterPos;


    event.updateTrans();

    return event;
}
