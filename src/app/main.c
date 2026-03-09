/*
 * Program: main
 * Model  : C driver for x86_64 ASM routines
 * Goal   : run assembly hello and mmap demos
 *
 * Usage: ./program [hello|anon|buf|file|all]
 *   hello - run ASM hello routine
 *   anon  - anonymous page mapping
 *   buf   - anonymous integer buffer
 *   file  - file-backed shared mapping
 *   all   - run all in order (default)
 */

#include <stdio.h>
#include <string.h>
#include "map.h"

extern void _hello(void);

int main(int argc, char **argv)
{
	const char *mode = (argc > 1) ? argv[1] : "all";
	int ret;

	if (strcmp(mode, "hello") != 0 &&
	    strcmp(mode, "anon")  != 0 &&
	    strcmp(mode, "buf")   != 0 &&
	    strcmp(mode, "file")  != 0 &&
	    strcmp(mode, "all")   != 0) {
		fprintf(stderr, "usage: %s [hello|anon|buf|file|all]\n", argv[0]);
		return 1;
	}

	if (strcmp(mode, "hello") == 0 || strcmp(mode, "all") == 0)
		_hello();

	if (strcmp(mode, "anon") == 0 || strcmp(mode, "all") == 0) {
		ret = anon();
		if (ret != 0) {
			fprintf(stderr, "anon: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (strcmp(mode, "buf") == 0 || strcmp(mode, "all") == 0) {
		ret = intbuf();
		if (ret != 0) {
			fprintf(stderr, "intbuf: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (strcmp(mode, "file") == 0 || strcmp(mode, "all") == 0) {
		ret = filemap();
		if (ret != 0) {
			fprintf(stderr, "filemap: %s\n", strerror(-ret));
			return 1;
		}
	}

	return 0;
}

