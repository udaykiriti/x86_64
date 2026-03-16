/*
 * Program: main
 * Model  : C driver for x86_64 ASM routines
 * Goal   : run assembly hello and mmap demos
 *
 * Usage: ./bin/program [hello|anon|buf|file|sysinfo|all]
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

enum run_mode {
	MODE_INVALID = 0,
	MODE_HELLO,
	MODE_ANON,
	MODE_BUF,
	MODE_FILE,
	MODE_SYSINFO,
	MODE_ALL,
};

static void usage(const char *name)
{
	fprintf(stderr,
		"usage: %s [hello|anon|buf|file|sysinfo|all]\n",
		name);
}

static enum run_mode parse_mode(const char *mode)
{
	if (mode == NULL)
		return MODE_INVALID;

	if (strcmp(mode, "hello") == 0)
		return MODE_HELLO;
	if (strcmp(mode, "anon") == 0)
		return MODE_ANON;
	if (strcmp(mode, "buf") == 0)
		return MODE_BUF;
	if (strcmp(mode, "file") == 0)
		return MODE_FILE;
	if (strcmp(mode, "sysinfo") == 0)
		return MODE_SYSINFO;
	if (strcmp(mode, "all") == 0)
		return MODE_ALL;

	return MODE_INVALID;
}

static int sysinfo(void)
{
	long page_size = sysconf(_SC_PAGESIZE);

	if (page_size <= 0)
		return -1;

	if (printf("sysinfo: page_size=%ld bytes, pointer_size=%zu bytes\n",
		   page_size, sizeof(void *)) < 0)
		return -1;

	return 0;
}

int main(int argc, char **argv)
{
	const char *mode_name = (argc > 1) ? argv[1] : "all";
	const long  first = 0x0003;
	const long  second = 0x0004;
	enum run_mode mode = parse_mode(mode_name);
	int ret;

	if (argc > 2) {
		usage(argv[0]);
		return 1;
	}

	if (mode == MODE_INVALID) {
		usage(argv[0]);
		return 1;
	}

	if (mode == MODE_HELLO || mode == MODE_ALL) {
		_hello();
		if (printf("_add(%ld, %ld) = %ld\n",
			   first, second, _add(first, second)) < 0)
			return 1;
		if (fflush(stdout) != 0)
			return 1;
	}

	if (mode == MODE_ANON || mode == MODE_ALL) {
		ret = anon();
		if (ret != 0) {
			fprintf(stderr, "anon: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_BUF || mode == MODE_ALL) {
		ret = intbuf();
		if (ret != 0) {
			fprintf(stderr, "intbuf: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_FILE || mode == MODE_ALL) {
		ret = filemap();
		if (ret != 0) {
			fprintf(stderr, "filemap: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_SYSINFO || mode == MODE_ALL) {
		ret = sysinfo();
		if (ret != 0) {
			fprintf(stderr, "sysinfo: failed to read system info\n");
			return 1;
		}
	}

	return 0;
}
