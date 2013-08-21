
#ifndef cppkit_files_h
#define cppkit_files_h

#include "cppkit/ck_string.h"

#ifdef IS_LINUX
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef IS_WINDOWS
#include <direct.h>
#endif

namespace cppkit
{

#ifdef IS_WINDOWS
const static char WRITE_MODE[] = "wb";
const static char READ_MODE[] = "rb";
#endif

#ifdef IS_LINUX
const static char READ_MODE[] = "rb, ccs=UTF-8";
const static char WRITE_MODE[] = "wb, ccs=UTF-8";
#endif

#ifdef IS_LINUX

inline int ck_mkdir(const ck_string& path)
{
	return mkdir(path.c_str(),0777);
}

inline FILE *ck_fopen( const ck_string& path, const ck_string& mode )
{
    FILE* fp;
    fp = fopen( path.c_str(), mode.c_str() );

    if ( !fp )
        CK_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}

#endif

#ifdef IS_WINDOWS

inline int ck_mkdir( const ck_string& path )
{
    return _wmkdir(path.get_wide_string().data());
}

inline FILE *ck_fopen( const ck_string& path, const ck_string& mode )
{
    errno_t err;
    FILE* fp;

    if ( err = _wfopen_s( &fp, path.get_wide_string().c_str(), mode.get_wide_string().c_str() ) != 0 )
        CK_THROW(( "fopen_s returned error code: %d.\n", (int)err ));

    if ( !fp )
        CK_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}

#endif

}

#endif
