#pragma once

#include <QtCore/qglobal.h>

#if defined(ORM_LIBRARY)
#  define ORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ORMSHARED_EXPORT Q_DECL_IMPORT
#endif


