/*=============================================================================
This is a Program to demonstrate a mouse over fly label with links
Copyright (C) 2015  Pradyumna Pramod Saraph
Contact: prady.pps@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Lesser GNU General Public License for more details.

You should have received a copy of the Lesser GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
=============================================================================*/
#include "FlyLinks.h"
#include <QHeaderView>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QMessageBox>
#include <QMap>

#ifndef DESTROY
#define DESTROY(control)        \
    if(control) {               \
    control->disconnect();  \
    delete control;         \
    control = NULL;         \
    }
#endif

FlyLinks::FlyLinks(QLabel *parent):
    QObject(parent),m_pObjLabel(parent),m_prevTreeItem(NULL){
    m_pObjPopup = new QTreeWidget(parent);
    MakeConnections();
    MakePopupVisible(false);
}

FlyLinks::~FlyLinks(){
    DESTROY(m_pObjPopup);
    m_pObjLabel = NULL;
    m_prevTreeItem = NULL;
}

void
FlyLinks::PopulateActionMap(
        const QMap<QString,QString>& objActionMap) {
    AssembleTreeWidget(objActionMap);
    MakePopupVisible(true);
}

void FlyLinks::AssembleTreeWidget(
        const QMap<QString,QString>& objDataMap) {
    m_pObjPopup->setWindowFlags(Qt::Popup);
    m_pObjPopup->setFocusPolicy(Qt::NoFocus);
    m_pObjPopup->setMouseTracking(true);
    m_pObjPopup->viewport()->setMouseTracking(true);
    m_pObjPopup->viewport()->installEventFilter(this);

    m_pObjPopup->setColumnCount(1);
    m_pObjPopup->setUniformRowHeights(true);
    m_pObjPopup->setRootIsDecorated(false);
    m_pObjPopup->setEditTriggers(QTreeWidget::NoEditTriggers);
    m_pObjPopup->setSelectionBehavior(QTreeWidget::SelectRows);
    m_pObjPopup->setFrameStyle(QFrame::Box | QFrame::Plain);
    m_pObjPopup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pObjPopup->header()->hide();
    m_pObjPopup->viewport()->
            setProperty("cursor",
                        QVariant(QCursor(Qt::PointingHandCursor)));

    m_pObjPopup->installEventFilter(this);
    PopulateTreeWidgetItems(objDataMap);
    //popup->setCurrentItem(popup->topLevelItem(0));
    m_pObjPopup->resizeColumnToContents(0);
    m_pObjPopup->adjustSize();
    m_pObjPopup->setUpdatesEnabled(true);

    int h = m_pObjPopup->sizeHintForRow(0) * (objDataMap.count()+1);
    //popup->resize(popup->width(), h);
    //popup->setMinimumHeight(h);
    m_pObjPopup->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pObjPopup->setMaximumHeight(h);
    QList<QString> objKeyList = objDataMap.keys();
    QListIterator<QString> objKeyIter(objKeyList);
    int iLenMax = 0,iLenCurrent = 0;
    while(objKeyIter.hasNext()) {
        iLenCurrent = objKeyIter.next().length();
        if(iLenCurrent > iLenMax) {
            iLenMax = iLenCurrent;
        }
    }
    QFontMetrics objMetrics(m_pObjPopup->font());
    m_pObjPopup->setMaximumWidth(objMetrics.maxWidth() * iLenMax); //adjust if necessary
    m_pObjPopup->resizeColumnToContents(0);
    m_pObjPopup->adjustSize();
    m_pObjPopup->setUpdatesEnabled(true);

}

void FlyLinks::MoveToParent(){
    m_pObjPopup->move(m_pObjLabel->
                      mapToGlobal(QPoint(0,m_pObjLabel->height())));
}

void FlyLinks::ProcessCurrentItem() {
    QTreeWidgetItem *item = m_pObjPopup->currentItem();
    if(item) {
        //We have a valid item
        QString strItemText = item->text(0);
        if(!ExecuteRqstMapping(item)) {
            MakePopupVisible(false);
            QMessageBox::critical(NULL,"Fly Popup","Unable to Invoke link");
        } else {
            MakePopupVisible(false);
        }
    }
}

bool FlyLinks::ExecuteRqstMapping(const QTreeWidgetItem *item) {
    if(item) {
        return QDesktopServices::openUrl(
                    QUrl(item->data(0,Qt::UserRole).toString(),
                         QUrl::TolerantMode));
    }
    return false;
}

void FlyLinks::PopulateTreeWidgetItems(
        const QMap<QString,QString>& objDataMap) {

    QMapIterator<QString,QString> objIterMap(objDataMap);
    QFont font;
    font.setPointSizeF(8.25);
    font.setUnderline(true);
    m_pObjPopup->setSortingEnabled(false);

    QBrush brush(QColor(85, 0, 255, 255));
    brush.setStyle(Qt::NoBrush);

    QTreeWidgetItem *pItem = new QTreeWidgetItem(m_pObjPopup);
    pItem = m_pObjPopup->headerItem();
    pItem->setText(0,tr("1"));
    pItem->setFont(0, font);
    pItem->setForeground(0, brush);
    pItem->setTextAlignment(0, Qt::AlignLeft);
    m_pObjTreeItemsVec.push_back(pItem);

    int iCount = 0;
    while(objIterMap.hasNext()) {
        objIterMap.next();
        QBrush brush(QColor(85, 0, 255, 255));
        brush.setStyle(Qt::NoBrush);
        QTreeWidgetItem *pItem = new QTreeWidgetItem(m_pObjPopup);
        pItem = m_pObjPopup->topLevelItem(iCount++);
        pItem->setText(0,objIterMap.key());
        pItem->setData(0,Qt::UserRole,objIterMap.value());
        pItem->setFont(0, font);
        pItem->setForeground(0, brush);
        pItem->setTextAlignment(0, Qt::AlignLeft);
    }
}


//the event filer...
//here we have to capture only the click event.
//Get the item clicked and exec a new Browser window based on the Link mapping
bool FlyLinks::eventFilter(QObject *obj, QEvent *ev) {
    if(obj == m_pObjPopup->viewport()) {
        if(ev->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(ev);
            QTreeWidgetItem* current =
                    m_pObjPopup->itemAt(mouseEvent->pos());
            if (current && current != m_prevTreeItem) {
                if (m_prevTreeItem) {
                    m_prevTreeItem->setBackground(0,
                                                  m_pObjPopup->palette().base()); // reset previous
                }
                current->setBackground(0, QColor(0, 0, 255, 50)); // highlight current
                if(current) {
                    m_prevTreeItem = current;
                }
            }
            return true;
        }
    }

    if (obj != m_pObjPopup) {
        return false;
    }

    if (ev->type() == QEvent::MouseButtonPress) {
        MakePopupVisible(false);
        if(m_pObjPopup->hasFocus()) {
            ProcessCurrentItem();
            m_pObjLabel->setFocus();
        }
        return true;
    }



    if (ev->type() == QEvent::Leave) {
        MakePopupVisible(false);
        m_pObjLabel->setFocus();
        return true;
    }

    if (ev->type() == QEvent::KeyPress) {
        bool consumed = false;
        int key = static_cast<QKeyEvent*>(ev)->key();
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            break;

        case Qt::Key_Escape:
            m_pObjLabel->setFocus();
            MakePopupVisible(false);
            consumed = true;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            m_pObjLabel->setFocus();
            MakePopupVisible(false);
            break;
        }
        return consumed;
    }
    return false;
}

void FlyLinks::MakeConnections() {
    connect(m_pObjPopup, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            SLOT(ProcessCurrentItem()));
}

void FlyLinks::MakePopupVisible(bool bVisible) {
    if(bVisible) {
        if(!m_pObjPopup->isVisible()) {
            m_pObjPopup->show();
            m_pObjPopup->setFocus();
        }
    } else {
        m_pObjLabel->setFocus();
        m_pObjPopup->hide();
    }
}
