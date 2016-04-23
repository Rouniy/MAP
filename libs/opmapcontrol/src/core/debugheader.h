#ifndef DEBUGHEADER_H
#define DEBUGHEADER_H

#ifndef QT_NO_DEBUG
#define LOCAL_ASSERT(x)  Q_ASSERT(x)
#else
#define LOCAL_ASSERT(x)  x
#endif

//#include "coreutils.h"

//#define DEBUG_MEMORY_CACHE
//#define DEBUG_CACHE
//#define DEBUG_GMAPS
//#define DEBUG_PUREIMAGECACHE
//#define DEBUG_TILECACHEQUEUE
//#define DEBUG_URLFACTORY
//#define DEBUG_MEMORY_CACHE
//#define DEBUG_GetGeocoderFromCache

#endif // DEBUGHEADER_H
