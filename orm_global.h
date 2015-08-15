#ifndef ORM_GLOBAL_H
#define ORM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ORM_LIBRARY)
#  define ORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ORM_GLOBAL_H

/*#include "beanquery.h"
#include "ormforeign.h"

void test() {
    BeanQuery<OrmForeign>* q=new BeanQuery<OrmForeign>();
    q->select()->
q->query();
}
*/
