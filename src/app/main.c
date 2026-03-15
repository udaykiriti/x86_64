/*
 * Program: main
 * Model  : C driver for x86_64 ASM routines
 * Goal   : run assembly hello and mmap demos
 *
 * Usage: ./program [hello|anon|buf|file|sysinfo|all]
 *   hello - run ASM hello routine
 *   anon  - anonymous page mapping
 *   buf   - anonymous integer buffer
 *   file  - file-backed shared mapping
 *   sysinfo - show basic runtime system information
 *   all   - run all in order (default)
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "map.h"

extern void _hello(void);
extern long _add(long a, long b);

static void usage(const char *name)
{
	fprintf(stderr,
		"usage: %s [hello|anon|buf|file|sysinfo|all]\n",
		name);
}

static int sysinfo(void)
{
	long page_size = sysconf(_SC_PAGESIZE);

	if (page_size <= 0)
		return 1;

	printf("sysinfo: page_size=%ld bytes, pointer_size=%zu bytes\n",
	       page_size, sizeof(void *));
	return 0;
}

int main(int argc, char **argv)
{
	const char *mode = (argc > 1) ? argv[1] : "all";
	const long  first = 0x0003;
	const long  second = 0x0004;
	int ret;

	if (strcmp(mode, "hello") != 0 &&
	    strcmp(mode, "anon")  != 0 &&
	    strcmp(mode, "buf")   != 0 &&
	    strcmp(mode, "file")  != 0 &&
	    strcmp(mode, "sysinfo") != 0 &&
	    strcmp(mode, "all")   != 0) {
		usage(argv[0]);
		return 1;
	}

	if (strcmp(mode, "hello") == 0 || strcmp(mode, "all") == 0) {
		_hello();
		printf("_add(%ld, %ld) = %ld\n",
		       first, second, _add(first, second));
	}

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

	if (strcmp(mode, "sysinfo") == 0 || strcmp(mode, "all") == 0) {
		ret = sysinfo();
		if (ret != 0) {
			fprintf(stderr, "sysinfo: failed to read system info\n");
			return 1;
		}
	}

	return 0;
}
