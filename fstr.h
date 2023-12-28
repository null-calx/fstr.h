// fstr.h
// A simple StringView implementation for C

// This is free and unencumbered software released into the public domain.

// Author: calx
// URL: https://github.com/null-calx/fstr.h
// Version: 0.0.1

// This is a simple implementation of StringView inspired from
// Tsoding's sv.h (https://github.com/tsoding/sv), but with some
// personal tweeks.

// USAGE:
//   #define FSTR_IMPLEMENTATION
//   #include "./fstr.h"

#ifndef FSTR_H_
#define FSTR_H_

#ifndef FSTRDEF
#define FSTRDEF
#endif // FSTRDEF

#include <stdbool.h>

typedef struct {
    size_t length;
    const char *start;
} fstr;

#define FSTR(cstr_lit) fstr_from_parts(cstr_lit, sizeof(cstr_lit) - 1)
#define FSTR_STATIC(cstr_lit) { sizeof(cstr_lit) - 1, (cstr_lit) }

#define FSTR_NULL fstr_from_parts(NULL, 0)

// printf macros for fstr
#define FSTR_FMT "%.*s"
#define FSTR_ARG(fs) (int) (fs).length, (fs).start
// USAGE:
//   fstr name = ...;
//   printf("Name: "FSTR_FMT"\n", FSTR_ARG(name));

FSTRDEF fstr fstr_from_parts(const char *start, size_t length);
FSTRDEF fstr fstr_copy(const fstr fs);

FSTRDEF fstr fstr_from_cstr(const char *cstr);
FSTRDEF char *fstr_to_cstr(const fstr fs);

FSTRDEF fstr fstr_from_fd(int fd);
FSTRDEF void fstr_free(fstr fs);

FSTRDEF fstr fstr_trim_left(const fstr fs);
FSTRDEF fstr fstr_trim_right(const fstr fs);
FSTRDEF fstr fstr_trim(const fstr fs);

FSTRDEF fstr fstr_chop_by_delim(const fstr fs, char delim);

#endif // FSTR_H_

#ifdef FSTR_IMPLEMENTATION

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

FSTRDEF fstr fstr_from_parts(const char *start, size_t length)
{
    fstr fs;
    fs.start = start;
    fs.length = length;
    return fs;
}

FSTRDEF fstr fstr_copy(const fstr fs)
{
    return fstr_from_parts(fs.start, fs.length);
}

FSTRDEF fstr fstr_from_cstr(const char *cstr)
{
    return fstr_from_parts(cstr, strlen(cstr));
}

FSTRDEF char *fstr_to_cstr(const fstr fs)
{
    // try allocating memory
    char *start = (char *) malloc(fs.length + 1);

    if (start != NULL) {
	memcpy(start, fs.start, fs.length);
	start[fs.length] = '\0';
    }
    return start;
}

FSTRDEF fstr fstr_from_fd(int fd)
{
    // try to get file length using fstat
    struct stat buf;
    int ret = fstat(fd, &buf);
    if (ret != 0) {
	return FSTR_NULL;
    }

    // try allocating memory
    int length = (int) buf.st_size;
    char *start = (char *) malloc(length);
    if (start == NULL) {
	return FSTR_NULL;
    }

    // try reading file
    ret = read(fd, start, length);
    if (ret != length) {
	free(start);
	return FSTR_NULL;
    }

    // success
    fstr fs = fstr_from_parts(start, length);
    return fs;
}

FSTRDEF void fstr_free(fstr fs)
{
    free((void *) fs.start);
}

FSTRDEF fstr fstr_trim_left(const fstr fs)
{
    size_t i = 0;
    while (i < fs.length && isspace(fs.start[i])) {
	++ i;
    }
    return fstr_from_parts(fs.start + i, fs.length - i);
}

FSTRDEF fstr fstr_trim_right(const fstr fs) {
    size_t i = 0;
    while (i < fs.length && isspace(fs.start[fs.length - i - 1])) {
	++ i;
    }
    return fstr_from_parts(fs.start, fs.length - i);
}

FSTRDEF fstr fstr_trim(const fstr fs) {
    return fstr_trim_right(fstr_trim_left(fs));
}

FSTRDEF fstr fstr_chop_by_delim(const fstr fs, char delim)
{
    size_t i = 0;
    while (i < fs.length && fs.start[i] != delim) {
	++ i;
    }
    return fstr_from_parts(fs.start, i);
}

#endif // FSTR_IMPLEMENTATION
