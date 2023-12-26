#include <stdio.h>
#include <fcntl.h>

#define FSTR_IMPLEMENTATION
#include "fstr.h"

int main(int argc, char **argv)
{
    // fprintf(stdout, "Hello, world!\n");
    if (argc < 2) {
	fprintf(stderr, "ERROR: filename not provided\n");
	return 1;
    }
    const char *pathname = argv[1];
    int fd = open(pathname, O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "ERROR: cannot open file: %s\n", pathname);
	return 1;
    }

    fstr file = fstr_from_fd(fd);
    fprintf(stdout, FSTR_FMT, FSTR_ARG(file));
    fstr_free(file);
    close(fd);
    return 0;
}
