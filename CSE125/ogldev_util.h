#ifndef OGLDEV_UTIL_H
#define	OGLDEV_UTIL_H

#pragma once
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;


using namespace std;

bool ReadFile(const char* fileName, string& outFile);

void OgldevFileError(const char* pFileName, uint line, const char* pFileError);

#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifdef WIN32
#define SNPRINTF _snprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
float fmax(float a, float b);
#else
#define SNPRINTF snprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long GetCurrentTimeMillis();

#endif	/* OGLDEV_UTIL_H */

