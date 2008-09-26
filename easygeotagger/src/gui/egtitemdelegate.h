#ifndef EGTITEMDELEGATE_H
#define EGTITEMDELEGATE_H

#include "egtexifio.h"

#include <QMap>
#include <QString>
#include <QItemDelegate>

class EgtItemDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  EgtItemDelegate();
  void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;

public slots:
  void displayGpsExifAvailability(int);

private:

//   struct cacheObject {
//     bool isImage;
//     bool hasGpsExif;
//   };

  bool cvDisplayGpsExifAvailability;
  //QMap< QString, EgtItemDelegate::cacheObject > cvExifCache;

  QString buildPath(const QModelIndex& theIndex) const;
};
#endif
