#include "../fileio.h"

#if defined(KLIB_CONFIG_FILEIO_WIN)

fileio* klib_fopen(const char* filename, const char* mode)
{
    if (!mode) return 0;
    bool error = false;
    fileio* fp = 0;

    unsigned int am = 0, cm = 0;
    fp = (fileio*)calloc(1, sizeof(fileio));
    fp->buf = 0;

    fp->bufsize = KLIB_FILEIO_BUFSIZE;
    fp->alcsize = KLIB_FILEIO_BUFSIZE;
    fp->buf = (char *)calloc(KLIB_FILEIO_BUFSIZE, sizeof(char));
    fp->allocated = true;
    fp->len = 0;

    char m0 = mode[0], m1 = 0;
    for (const char* p = mode + 1; *p; ++p) {
        if (*p == '+') m1 = 1;
        if (*p == 'b') fp->flags |= 1;
    }

    switch (m0) {
    case 'r':
        am = m1 ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
        cm = OPEN_EXISTING;
        break;
    case 'w':
        am = m1 ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_WRITE;
        cm = CREATE_ALWAYS;
        break;
    case 'a':
        am = m1 ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_WRITE;
        cm = OPEN_ALWAYS;
        break;
    }

    if (!am) error = true;
    if (!error) {
        fp->handle = CreateFileA(filename, am, 0, NULL, cm, FILE_ATTRIBUTE_NORMAL, 0);
        if (fp->handle == INVALID_HANDLE_VALUE) {
            error = true;
        }
    }
    if (!error) {
        if (m0 == 'a') {
            if (SetFilePointer(fp->handle, 0, NULL, FILE_END) == 0xffffffff) {
                error = true;
            }
        }
    }

    if (error) {
        if (fp->handle != INVALID_HANDLE_VALUE) {
            CloseHandle(fp->handle);
        }
        if (fp->allocated) {
            free(fp->buf);
        }
        free(fp);
        return 0;
    }

    return fp;
}

#endif
