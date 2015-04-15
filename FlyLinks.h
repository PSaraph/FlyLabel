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
#ifndef FLYLINKS_H
#define FLYLINKS_H
#include <QObject>
#include <QTreeWidget>
#include <QLabel>
#include <QMap>

class FlyLinks : public QObject {
    Q_OBJECT

public:
    FlyLinks(QLabel *parent = 0);
    ~FlyLinks();
    void PopulateActionMap(const QMap<QString,QString>& objActionMap);
    void MakePopupVisible(bool bVisible = true);
    void MoveToParent();

public slots:
    void ProcessCurrentItem();
private:
    void AssembleTreeWidget(const QMap<QString, QString> &objDataMap);
    void PopulateTreeWidgetItems(
            const QMap<QString,QString>& objDataMap);
    bool ExecuteRqstMapping(const QTreeWidgetItem *item);
    void MakeConnections();
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:

    QTreeWidget *m_pObjPopup;
    QLabel* m_pObjLabel;
    QVector<QTreeWidgetItem *> m_pObjTreeItemsVec;
    QTreeWidgetItem* m_prevTreeItem;
};
#endif // FLYLINKS_H
