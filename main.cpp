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
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include "FlyLinksLabel.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    FlyLinksLabel* label = new FlyLinksLabel("Links",NULL);
    QMap<QString,QString> objLinksMap;
    objLinksMap.insert("google","www.google.com");
    objLinksMap.insert("yahoo","www.yahoo.com");
    objLinksMap.insert("flipkart","www.flipkart.com");
    objLinksMap.insert("ebay","www.ebay.com");
    label->AddLinks(objLinksMap);
    QWidget* widget=new QWidget;

    // create horizontal layout
    QHBoxLayout* layout=new QHBoxLayout;
    // and add label to it
    layout->addWidget(label);
    // set layout to widget
    widget->setLayout(layout);

    widget->show();
    return app.exec();
}
