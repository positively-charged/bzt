/*

   Here I put useful file-related functions I made.

*/

#ifndef FILEUTIL_H
#define FILEUTIL_H

#ifdef _WIN32
   #define FILE_PATH_SEPARATOR '\\'
#else
   #define FILE_PATH_SEPARATOR '/'
#endif

/* Function to fine the basename of a file path. */
const char *FileBasename( const char *filePath );

#endif
