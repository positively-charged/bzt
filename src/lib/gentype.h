/*

   Useful types to be used throughout the program.

*/

#ifndef LIB_GENTYPE_H
#define LIB_GENTYPE_H

// Platform detection.
#if defined( _WIN32 ) || defined( _WIN64 )
#   define OS_WINDOWS
#else
#   define OS_LINUX
#endif

#endif