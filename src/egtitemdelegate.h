#ifndef EGTITEMDELEGATE_H
#define EGTITEMDELEGATE_H

#include "egtexifio.h"

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
  bool cvDisplayGpsExifAvailability;


};
#endif
