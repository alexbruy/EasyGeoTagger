#include <QDebug>

#ifdef EGTDEBUG
#define EgtDebug(theMessage) qDebug("%s [%d]: %s", __FILE__, __LINE__, theMessage);
#else
# define EgtDebug(theMessage)
#endif
