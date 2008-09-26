#include "egtitemdelegate.h"

#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QDir>

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

  if(cvDisplayGpsExifAvailability)
  {
    /*
    QString lvFileName = buildPath( theIndex );
    EgtItemDelegate::cacheObject lvCacheObject;
    lvCacheObject.isImage = false;
    lvCacheObject.hasGpsExif = false;
        
    if( cvExifCache.contains( lvFileName ) )
    {
      qDebug("Using Cached value");
      lvCacheObject = cvExifCache.value( lvFileName );
      if( lvCacheObject.isImage )
      {
        if( lvCacheObject.hasGpsExif )
        {
          QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
          lvPixmap.fill( QColor( 0,255,0,75 ) );
          thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
        }
        else
        {
          QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
          lvPixmap.fill( QColor( 255,0,0,75 ) );
          thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
        }
      }
    }
    else
    {
    */
      EgtExifIO lvExifIO( theIndex );
      if( lvExifIO.isValidImage() )
      {
//         lvCacheObject.isImage = true;
        if( lvExifIO.hasGpsExif() )
        {
//           lvCacheObject.hasGpsExif = true;
          QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
          lvPixmap.fill( QColor( 0,255,0,75 ) );
          thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
        }
        else
        {
          QPixmap lvPixmap( lvViewOption.rect.x(),lvViewOption.rect.y() );
          lvPixmap.fill( QColor( 255,0,0,75 ) );
          thePainter->drawPixmap( lvViewOption.rect, lvPixmap );
        }
      }
//     }
//     cvExifCache.insert( lvFileName, lvCacheObject );
  }

  QItemDelegate::paint(thePainter, lvViewOption, theIndex);
}

/*
 *
 * PRIVATE FUNCTIONS
 *
 */
QString EgtItemDelegate::buildPath(const QModelIndex& theIndex) const
{
  if(!theIndex.isValid()) { return ""; }
  
  if(!theIndex.parent().isValid())
  {
    #ifdef WIN32
    return theIndex.data().toString();
    #else
    return "";
    #endif
  }
  
  return buildPath( theIndex.parent() ) + QDir::toNativeSeparators( "/" ) + theIndex.data().toString();
}
