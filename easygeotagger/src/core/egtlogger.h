#include <QDebug>
#include <QString>

#ifdef EGTDEBUG
#define EgtDebug( theMessage ) qDebug("%s [%d]: %s", __FILE__, __LINE__, qPrintable( QString( theMessage ) ) );
#else
# define EgtDebug( theMessage )
#endif
