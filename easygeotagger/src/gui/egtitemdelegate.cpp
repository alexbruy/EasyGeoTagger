#include "egtitemdelegate.h"

#include <QPainter>
#include <QPixmap>
#include <QColor>

EgtItemDelegate::EgtItemDelegate()
{
  cvDisplayGpsExifAvailability = false;
}

void EgtItemDelegate::displayGpsExifAvailability(int theState)
{
  if(theState == 0)
  {
    cvDisplayGpsExifAvailability = false;
  }
  else
  {
    cvDisplayGpsExifAvailability = true;
  }
}

void EgtItemDelegate::paint(QPainter* thePainter, const QStyleOptionViewItem& theOption, const QModelIndex& theIndex ) const
{

  QStyleOptionViewItem lvViewOption(theOption);
/*
  if(cvDisplayGpsExifAvailability)
  {
    EgtExifIO lvExifIO;
    
    if(lvExifIO.isValidImage(theIndex))
    {
      if(lvExifIO.hasGpsExif(theIndex))
      {
        QPixmap lvPixmap(lvViewOption.rect.x(),lvViewOption.rect.y());
        lvPixmap.fill(QColor(0,255,0,75));
        thePainter->drawPixmap(lvViewOption.rect, lvPixmap);
        //lvViewOption.palette.setColor(QPalette::Text, Qt::green);
      }
      else
      {
        QPixmap lvPixmap(lvViewOption.rect.x(),lvViewOption.rect.y());
        lvPixmap.fill(QColor(255,0,0,75));
        thePainter->drawPixmap(lvViewOption.rect, lvPixmap);
        //lvViewOption.palette.setColor(QPalette::Text, Qt::red);
      }
      
    }
    else if(!lvExifIO.isDirectory(theIndex))
    {
      lvViewOption.palette.setColor(QPalette::Text, Qt::gray);
    }
  }
*/  
  QItemDelegate::paint(thePainter, lvViewOption, theIndex);
}

