/*
** File: 
** Author(s): 
** Creation Date: 
**
** Copyright (c) 2008, American Museum of Natural History. All rights reserved.
**
** This library/program is free software; you can redistribute it
** and/or modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library/program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** This work was made possible through a grant by the the John D. and
** Catherine T. MacArthur Foundation with additional support from the
** Center For Biodiversity and Conservation and The Spanish Ministry of
** Science and Innovation's INTEGRANTS program.
**
**/
#include "egtgpsdatatable.h"

#include <QVBoxLayout>


EgtGpsDataTable::EgtGpsDataTable( )
{
  setLayout( new QVBoxLayout() );
  layout()->setContentsMargins( 1, 1, 1, 1 );

  cvTable = new QTableWidget(this);
  cvTable->setRowCount(10);
  cvTable->setColumnCount(3);

  QStringList lvTags;
  lvTags << "Longitude" << "Latitude" << "Altitude";
  cvTable->setHorizontalHeaderLabels(lvTags);

  cvMapItems = new QMap<QString,QString>;

  //Populate the table with some data
  populateTable();

  layout()->addWidget( cvTable );

  cvTagButton.setText( tr( "Tag picture" ) );
  layout()->addWidget( &cvTagButton );
  connect( &cvTagButton, SIGNAL( clicked() ), this, SLOT( cvTagButton_clicked() ) );

  connect( cvTable, SIGNAL( cellClicked(int, int) ), this, SLOT( cell_selected(int, int) ) );  

  cvHorizontalHeader = cvTable->horizontalHeader();
  connect( cvHorizontalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvHorizontalHeader_clicked(int) ) ); 

  cvVerticalHeader = cvTable->verticalHeader();
  connect( cvVerticalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvVerticalHeader_clicked(int) ) ); 
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QMap<QString,QString>* EgtGpsDataTable::getRowItems()
{
  return cvMapItems;
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

void EgtGpsDataTable::populateTable()
{
  for( int lvRowCounter= 0; lvRowCounter<10; lvRowCounter++ )
    for( int lvColumnCounter= 0; lvColumnCounter<3; lvColumnCounter++ )
    {
      QTableWidgetItem *lvNewItem = new QTableWidgetItem(tr("%1").arg(
         (lvRowCounter+1)*(lvColumnCounter+1)));
       cvTable->setItem( lvRowCounter, lvColumnCounter, lvNewItem );
    }
}
/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtGpsDataTable::cvHorizontalHeader_clicked( int theIndex )
{
qDebug(("Horizontal header clicked: "+QString::number(theIndex)).toStdString().c_str());
}

void EgtGpsDataTable::cvVerticalHeader_clicked(int theIndex)
{ 
  delete cvMapItems;
  cvMapItems = new QMap<QString,QString>;
 
  QTableWidgetItem* lvHeaderItem;
  for(int lvColumnCount = 0; lvColumnCount < cvTable->columnCount(); lvColumnCount++)
  {
    QString lvText;
    lvHeaderItem = cvTable->horizontalHeaderItem ( lvColumnCount );
    lvText = lvHeaderItem->text();
    cvMapItems->insert( lvText, cvTable->item( theIndex, lvColumnCount )->text() );
  } 
}


void EgtGpsDataTable::cell_selected(int row, int column)
{ 
  QTableWidgetItem * lvItem =cvTable->item ( row, column );
  double x = lvItem->data(0).toDouble();
  qDebug(("Cell clicked: "+QString::number(x)).toStdString().c_str());
}

void EgtGpsDataTable::cvTagButton_clicked()
{
  emit( rowSelected() );
}

/*void EgtGpsDataTable::void setFileReader()
{
}*/

