
#ifndef CNX_PATH
#define CNX_PATH

#include <Cnx/Def.h>
#include <Cnx/String.h>

/// @brief A `CnxPath` is a filesystem path to a file or directory
typedef struct CnxPath {
	/// the string representation of the path
	CnxString m_path;
} CnxPath;

#endif // CNX_PATH
