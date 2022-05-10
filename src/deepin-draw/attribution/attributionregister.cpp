#include "attributionregister.h"
#include "pageitem.h"
#include "drawboard.h"
#include "groupbuttonwidget.h"
#include "rotateattriwidget.h"
#include "orderwidget.h"
#include "attributemanager.h"
#include "pagescene.h"
#include "pageview.h"
#include "adjustmentatrriwidget.h"
#include "cundoredocommand.h"
#include "layeritem.h"
#include "styleattriwidget.h"
#include "colorstylewidget.h"
#include "csidewidthwidget.h"
#include "rectradiusstylewidget.h"

#include <QPoint>
#include <QObject>

AttributionRegister::AttributionRegister(DrawBoard *d): m_drawBoard(d)
{

}

void AttributionRegister::registe()
{
    registeBaseStyleAttrri();
    registeGroupAttri();
    resgisteRotateAttri();
    registeOrderAttri();
    registeAdjustImageAttri();
    registeStyleAttri();

    connect(m_drawBoard, qOverload<Page *>(&DrawBoard::currentPageChanged), this, [ = ](Page * page) {
        if (page->scene()->selectedItemCount()) {
            m_rotateAttri->setEnabled(false);
            m_groupWidget->setEnabled(false);
            m_orderAttri->setEnabled(false);
        }
    });
}

void AttributionRegister::registeGroupAttri()
{
    m_groupWidget = new GroupButtonWidget;
    setWgtAccesibleName(m_groupWidget, "groupButtonWidget");
    m_drawBoard->attributionManager()->installComAttributeWgt(EGroupWgt, m_groupWidget);

    connect(m_groupWidget, &GroupButtonWidget::buttonClicked, this, [ = ](bool doGroup, bool doUngroup) {
        auto currentPage = m_drawBoard->currentPage();
        if (currentPage == nullptr)
            return;

        auto currentScene = currentPage->scene();
        if (doGroup) {
            auto tmpGroupItem = currentScene->creatGroup(currentScene->selectedPageItems(), nullptr);
            currentScene->clearSelections();
            currentScene->selectPageItem(tmpGroupItem);
            //groupWidget->setGroupFlag(false, true);
        }

        if (doUngroup) {
            auto selectedItems = currentScene->selectedPageItems();
            if (!selectedItems.isEmpty()) {
                for (auto item : selectedItems) {
                    if (item->type() == GroupItemType) {
                        auto gp = static_cast<GroupItem *>(item);
                        currentScene->cancelGroup(gp);
                    }
                }
//                auto item = selectedItems.first();
//                if (item->type() == GroupItemType) {
//                    auto gp = static_cast<GroupItem *>(item);
//                    //UndoRecorder recorder(currentScene->currentTopLayer(), LayerUndoCommand::ChildGroupRemoved,
//                    //QList<PageItem *>() << gp << gp->items());
//                    currentScene->cancelGroup(gp);
//                }
            }

//            auto tmpGroupItem = dynamic_cast<GroupItem *>(currentScene->selectedPageItems().first());
//            QList<PageItem *> childItems = tmpGroupItem->items();
//            currentScene->cancelGroup(tmpGroupItem);
//            currentScene->selectPageItem(childItems);
//            groupWidget->setGroupFlag(true, false);
        }
        //另外需要将焦点转移到text
        auto pView = m_drawBoard->currentPage()->view();
        pView->setFocus();
        //pView->captureFocus();
    });

    connect(m_drawBoard->attributionManager()->helper(), &AttributionManagerHelper::updateWgt, this,
    [ = ](QWidget * pWgt, const QVariant & var) {
        if (pWgt == m_groupWidget) {
            bool canGroup = false;
            bool canUnGroup = false;
            QList<QVariant> bools = var.toList();
            if (bools.count() == 2) {
                canGroup   = bools[0].toBool();
                canUnGroup = bools[1].toBool();
            }

            m_groupWidget->setGroupFlag(canGroup, canUnGroup);
        }
    });
}

void AttributionRegister::resgisteRotateAttri()
{
    m_rotateAttri = new RotateAttriWidget(m_drawBoard);
    setWgtAccesibleName(m_rotateAttri, "rotateAttriWidget");
    m_drawBoard->attributionManager()->installComAttributeWgt(ERotProperty, m_rotateAttri, 0);
}

void AttributionRegister::registeOrderAttri()
{
    m_orderAttri = new OrderWidget(m_drawBoard);
    setWgtAccesibleName(m_orderAttri, "orderWidget");
    m_drawBoard->attributionManager()->installComAttributeWgt(EOrderProperty, m_orderAttri, 0);
}

void AttributionRegister::registeAdjustImageAttri()
{
    AdjustmentAtrriWidget *widget = new AdjustmentAtrriWidget;
    connect(widget->button(), &QPushButton::clicked, widget, [ = ]() {
        auto page = m_drawBoard->currentPage();
        if (page == nullptr)
            return;

        auto curScene = page->scene();
        if (curScene != nullptr) {
            auto rect = curScene->selectedPageItems().first()->sceneBoundingRect();
            if (rect != curScene->sceneRect()) {
                UndoRecorder block(page->scene()->currentTopLayer(), LayerUndoCommand::RectChanged);
                page->scene()->currentTopLayer()->setRect(rect);
                page->view()->setSceneRect(rect);
                widget->button()->setFlat(true);
            }
        }
    });

    connect(m_drawBoard->attributionManager()->helper(), &AttributionManagerHelper::updateWgt, this,
    [ = ](QWidget * pWgt, const QVariant & var) {
        if (pWgt == widget) {
            widget->setEnabled(var.toBool());
        }
    });

    m_drawBoard->attributionManager()->installComAttributeWgt(EImageAdaptScene, widget, false);
}

void AttributionRegister::registeStyleAttri()
{
    StyleAttriWidget *styleAtti = new StyleAttriWidget(EStyleProper, nullptr);
    styleAtti->addChildAtrri(m_fillStyle);
    styleAtti->addChildAtrri(m_borderStyle);
    styleAtti->addChildAtrri(m_rectRadius);
    m_penWidth->show();
    m_fillStyle->setProperty(ChildAttriWidget, true);
    m_borderStyle->setProperty(ChildAttriWidget, true);
    m_penWidth->setProperty(ChildAttriWidget, true);
    m_drawBoard->attributionManager()->installComAttributeWgt(styleAtti->attribution(), styleAtti, QVariant());
    m_rectRadius->setProperty(ChildAttriWidget, true);
}

void AttributionRegister::registeBaseStyleAttrri()
{
    m_fillStyle = new ColorStyleWidget;
    m_fillStyle->setColorFill(0);
    //m_borderStyle = new BorderStyleWidget;
    m_borderStyle = new ColorStyleWidget;
    m_borderStyle->setTitleText(tr("border"));
    m_borderStyle->setColorFill(1);
    m_borderStyle->setColorTextVisible(false);
    m_drawBoard->attributionManager()->installComAttributeWgt(EBrushColor, m_fillStyle, QColor(0, 0, 0));
    m_drawBoard->attributionManager()->installComAttributeWgt(EPenColor, m_borderStyle, QColor(0, 0, 0));

    connect(m_fillStyle, &ColorStyleWidget::colorChanged, this, [ = ](const QColor & color, int phase) {
        if (m_fillStyle->isEnableAttriVisible()) {
            QVariantList l;
            l << m_fillStyle->isAttriApply() << color;
            m_drawBoard->setDrawAttribution(EBrushColor, l, phase);
        } else {
            m_drawBoard->setDrawAttribution(EBrushColor, color, phase);
        }

    });

    connect(m_borderStyle, &ColorStyleWidget::colorChanged, this, [ = ](const QColor & color, int phase) {
        if (m_fillStyle->isEnableAttriVisible()) {
            QVariantList l;
            l << m_fillStyle->isAttriApply() << color;
            m_drawBoard->setDrawAttribution(EPenColor, l, phase);
        } else {
            m_drawBoard->setDrawAttribution(EPenColor, color, phase);
        }
    });

    m_penWidth = new SideWidthWidget;
    m_penWidth->setMinimumWidth(90);
    QObject::connect(m_penWidth, &SideWidthWidget::widthChanged, m_penWidth, [ = ](int width, bool preview = false) {
        Q_UNUSED(preview)
        m_drawBoard->setDrawAttribution(EPenWidth, width);
    });

    connect(m_drawBoard->attributionManager()->helper(), &AttributionManagerHelper::updateWgt, m_penWidth,
    [ = ](QWidget * pWgt, const QVariant & var) {
        if (pWgt == m_penWidth) {
            QSignalBlocker bloker(m_penWidth);
            int width = var.isValid() ? var.toInt() : -1;
            m_penWidth->setWidth(width);
        }
    });
    m_drawBoard->attributionManager()->installComAttributeWgt(EPenWidth, m_penWidth, 2);

    //在border中显示
    m_borderStyle->addWidget(m_penWidth);

    m_rectRadius = new RectRadiusStyleWidget;
    m_drawBoard->attributionManager()->installComAttributeWgt(m_rectRadius->attribution(), m_rectRadius, QVariant());
    QObject::connect(m_rectRadius, &RectRadiusStyleWidget::valueChanged, m_penWidth, [ = ](QVariant value, EChangedPhase phase) {
        m_drawBoard->setDrawAttribution(m_rectRadius->attribution(), value, phase);
    });
}
