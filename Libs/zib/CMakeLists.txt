#START-FILE-LIST
# File List auto generated by waf2cmake.py, do not modify manually.
set(LibName zlib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND
	zlib
		"../../SDKs/zlib-1.2.11/adler32.c"
		"../../SDKs/zlib-1.2.11/compress.c"
		"../../SDKs/zlib-1.2.11/crc32.c"
		"../../SDKs/zlib-1.2.11/crc32.h"
		"../../SDKs/zlib-1.2.11/deflate.c"
		"../../SDKs/zlib-1.2.11/deflate.h"
		"../../SDKs/zlib-1.2.11/gzclose.c"
		"../../SDKs/zlib-1.2.11/gzguts.h"
		"../../SDKs/zlib-1.2.11/gzlib.c"
		"../../SDKs/zlib-1.2.11/gzread.c"
		"../../SDKs/zlib-1.2.11/gzwrite.c"
		"../../SDKs/zlib-1.2.11/infback.c"
		"../../SDKs/zlib-1.2.11/inffast.c"
		"../../SDKs/zlib-1.2.11/inffast.h"
		"../../SDKs/zlib-1.2.11/inffixed.h"
		"../../SDKs/zlib-1.2.11/inflate.c"
		"../../SDKs/zlib-1.2.11/inflate.h"
		"../../SDKs/zlib-1.2.11/inftrees.c"
		"../../SDKs/zlib-1.2.11/inftrees.h"
		"../../SDKs/zlib-1.2.11/trees.c"
		"../../SDKs/zlib-1.2.11/trees.h"
		"../../SDKs/zlib-1.2.11/uncompr.c"
		"../../SDKs/zlib-1.2.11/zconf.h"
		"../../SDKs/zlib-1.2.11/zlib.h"
		"../../SDKs/zlib-1.2.11/zutil.c"
		"../../SDKs/zlib-1.2.11/zutil.h"
)

set(zlibFiles ${zlib})


if(WIN32 OR WIN64)
	add_library(${LibName} STATIC
		${zlibFiles}
	)
endif()

set_property(TARGET ${LibName} PROPERTY FOLDER "libs")
