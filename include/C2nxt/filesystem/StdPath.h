#include "../StdResult.h"
#include "../StdString.h"
#include "../StdVector.h"

#ifndef STD_PATH
	#define STD_PATH

/// @brief A `StdPath` is a filesystem path to a file or directory
typedef struct StdPath {
	/// the string representation of the path
	StdString m_path;
} StdPath;

typedef StdPath* Ref(StdPath);
typedef const StdPath* ConstRef(StdPath);

DeclStdOption(StdPath);
DeclStdResult(StdPath);
DeclStdIterators(Ref(StdPath));
DeclStdIterators(ConstRef(StdPath));
DeclStdVector(StdPath);

#endif
