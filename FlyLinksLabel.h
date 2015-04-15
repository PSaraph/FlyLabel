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
#ifndef FLYLINKSLABEL_H
#define FLYLINKSLABEL_H
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QTreeWidget>
#include "FlyLinks.h"

#ifndef DESTROY
#define DESTROY(control)        \
    if(control) {               \
    control->disconnect();  \
    delete control;         \
    control = NULL;         \
    }
#endif

class FlyLinksLabel : public QLabel {
    Q_OBJECT
public:
    explicit FlyLinksLabel(const QString strText,QWidget* pWidget = 0);
    virtual ~FlyLinksLabel();
    void AddLinks(const QMap<QString,QString>& objLinksMap);

protected:
    void enterEvent(QEvent* e);
private:
    FlyLinks* m_pObjPopUpList;
};
#endif // FLYLINKSLABEL_H
