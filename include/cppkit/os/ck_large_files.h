
#ifndef cppkit_large_files_h
#define cppkit_large_files_h

#define _LARGE_FILE_API
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cppkit/os/ck_exports.h"
#include "cppkit/ck_types.h"
#include "cppkit/ck_string.h"

#ifdef IS_LINUX
#include <unistd.h>
#endif

namespace cppkit
{

enum ck_file_type
{
    CK_REGULAR,
    CK_DIRECTORY
};

struct ck_file_info
{
    int64_t file_size;
    uint64_t access_time;
    ck_file_type file_type;
    uint32_t optimal_block_size;
};

CK_API int ck_stat( const ck_string& fileName, struct ck_file_info* fileInfo );

CK_API int ck_fseeko( FILE* file, int64_t offset, int whence );

CK_API int64_t ck_ftello( FILE* file );

CK_API int ck_filecommit( FILE* file );

CK_API int ck_fallocate( FILE* file, int64_t size );

/// Returns whether the given file exists.
CK_API bool ck_exists( const ck_string& fileName );

/// Returns a directory for putting temporary files.
CK_API ck_string ck_temp_dir();

}

#endif
