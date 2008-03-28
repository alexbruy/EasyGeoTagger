#ifndef EGTITEMDELEGATE_H
#define EGTITEMDELEGATE_H

#include "egtexifio.h"

#include <QItemDelegate>

class EgtItemDelegate : public QItemDelegate
{

public:
  EgtItemDelegate();
  void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const;

};
#endif
