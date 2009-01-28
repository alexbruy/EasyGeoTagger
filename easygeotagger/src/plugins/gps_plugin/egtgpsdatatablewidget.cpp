/*
** File: egtgpsdatatablewidget.cpp
** Author(s): Roberto Garcia Yunta
** Creation Date: 2008-12-19
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
#include "egtgpsdatatablewidget.h"

#include <QMessageBox>

EgtGpsDataTableWidget::EgtGpsDataTableWidget( )
{
  cvUiColumnMeaning.setupUi( &cvColumnMeaningDialog );

  QStringList lvAvailableFields;
  lvAvailableFields<<"Longitude"<<"Latitude"<<"Altitude";
  cvUiColumnMeaning.cbFields->insertItems(0,lvAvailableFields);

  cvColumnSelected = 0;
  cvHeadersAreSet = false;

  cvMapItems = new QMap<QString,QString>;

  connect( this, SIGNAL( cellClicked(int, int) ), this, SLOT( cell_selected(int, int) ) );  

  cvHorizontalHeader = horizontalHeader();
  connect( cvHorizontalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvHorizontalHeader_clicked(int) ) ); 

  cvVerticalHeader = verticalHeader();
  connect( cvVerticalHeader, SIGNAL( sectionClicked( int ) ), this, SLOT( cvVerticalHeader_clicked(int) ) ); 

  connect( cvUiColumnMeaning.pbtnOk, SIGNAL( clicked() ), this, SLOT(on_pbtnOk_clicked() ) );
}

/*
 *
 * PUBLIC FUNCTIONS
 *
 */

QMap<QString,QString>* EgtGpsDataTableWidget::getRowItems()
{
  return cvMapItems;
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */

bool EgtGpsDataTableWidget::areAllTheColumnsSet()
{
  if( cvHeadersAreSet ) return true;

  bool lvReturn = true;
  for( int i = 0; i < columnCount(); i++ )
  {
    lvReturn = lvReturn && cvHeadersThatAreSet.contains( i );
  }
  cvHeadersAreSet = lvReturn;  

  return lvReturn;
}

void EgtGpsDataTableWidget::populateTable()
{
  cvHeadersThatAreSet.clear();

  QList<QStringList> lvDataFile =cvFileReader->read();

  setRowCount( lvDataFile.size() );
  setColumnCount( lvDataFile[0].size() );

  if( cvFileReader->hasColumnHeaders() ) 
  {
    QStringList lvTags;
    lvTags = cvFileReader -> columnHeaders();
    setHorizontalHeaderLabels( lvTags );
  }
  else
  {
    QStringList lvDefaultHeader;
 
    for(int i = 0; i< lvDataFile[0].size(); i++ )
      lvDefaultHeader << QString::number( i+1 );

    setHorizontalHeaderLabels( lvDefaultHeader );
  }

  for(int i = 0; i < lvDataFile.size(); i++ )
    for(int j = 0; j < lvDataFile[0].size(); j++ )
    {
      QTableWidgetItem *lvNewItem = new QTableWidgetItem(lvDataFile.at(i).at(j));
       setItem( i, j, lvNewItem );
    }
}


/*
 *
 * SIGNAL and SLOTS
 *
 */

void EgtGpsDataTableWidget::cell_selected(int row, int column)
{ 
  QTableWidgetItem * lvItem =item ( row, column );
  double x = lvItem->data(0).toDouble();
}

void EgtGpsDataTableWidget::cvHorizontalHeader_clicked( int theIndex )
{
  cvColumnSelected = theIndex;
  cvColumnMeaningDialog.show();
}

void EgtGpsDataTableWidget::cvVerticalHeader_clicked( int theIndex )
{ 
  delete cvMapItems;
  cvMapItems = new QMap<QString,QString>;
 

  if( areAllTheColumnsSet() )
  {
    QTableWidgetItem* lvHeaderItem;
    for(int lvColumnCount = 0; lvColumnCount < columnCount(); lvColumnCount++)
    {
      QString lvText;
      lvHeaderItem = horizontalHeaderItem ( lvColumnCount );
      lvText = lvHeaderItem->text();
      cvMapItems->insert( lvText, item( theIndex, lvColumnCount )->text() );
    }
  }
  else
  {
    QMessageBox::critical( this, tr("Error"),tr("All headers must be set"),QMessageBox::Ok );
  }
}

void EgtGpsDataTableWidget::on_pbtnOk_clicked()
{   
  QString lvSelectedItem = cvUiColumnMeaning.cbFields->currentText();
  setHorizontalHeaderItem(cvColumnSelected, new QTableWidgetItem( lvSelectedItem ));
  cvHeadersThatAreSet<<cvColumnSelected;
  cvColumnMeaningDialog.close();
}

void EgtGpsDataTableWidget::setFileReader(EgtFileReader* theFileReader)
{
  cvFileReader= theFileReader;
  cvHeadersAreSet = cvFileReader->hasColumnHeaders();
  populateTable();
}
