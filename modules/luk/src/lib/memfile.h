/*

   This library helps us with manipulating files in memory.
   -- Positron

*/

#ifndef MEMFILE_H
#define MEMFILE_H

#include <stdlib.h>

#include "gentype.h"

#define MF_START_MEMORY 1024

/* Error codes for errors that might be encountered when manipulating
   memory files. */
#define MF_ERR_OUT_OF_MEMORY -1
#define MF_ERR_BAD_PATH -2
#define MF_ERR_FILE_READ -3
#define MF_ERR_FILE_WRITE -4
#define MF_ERR_INVALID_POSITION -5

/* When loading a file to append to a memory file, we will read the file
   data into a temporary buffer before adding in to the memory file. */
#define MF_TEMP_BUFFER_SIZE MF_START_MEMORY

typedef struct {
   Byte *data;
   /* Size of allocated memory in bytes. */
   size_t memoryAllocated;
   /* Size of all the data currently in the memory file. */
   size_t size;
   /* Variable to point to a position in the data. */
   size_t pos;
} MemFile;

void MemFileInit( MemFile *memFile );
int MemFileAdd( MemFile *memFile, const void *data, size_t numberOfBytes );
int MemFileAddFile( MemFile *memFile, const char *filePath );
int MemFileAddMemFile( MemFile *memFile, const MemFile *otherMemFile );
int MemFileSave( const MemFile *memFile, const char *outPath );
size_t MemFileRead( MemFile *memFile, void *buffer, size_t numberOfBytes );
int MemFileSetPosition( MemFile *memFile, size_t newPosition );
size_t MemFileGetPosition( const MemFile *memFile );
size_t MemFileGetSize( const MemFile *memFile );
void MemFileRewind( MemFile *memFile );
const char *MemFileGetErrorCodeMessage( int errorCode );
void MemFilePrint( const MemFile *memFile, Bool printData );
void MemFileClose( MemFile *memFile );

#endif
