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

enum mode {
	MODE_INVALID = 0,
	MODE_HELLO,
	MODE_ANON,
	MODE_BUF,
	MODE_FILE,
	MODE_SYSINFO,
	MODE_ALL,
};

struct entry {
	const char *name;
	enum mode id;
};

static const struct entry modes[] = {
	{ "hello", MODE_HELLO },
	{ "anon", MODE_ANON },
	{ "buf", MODE_BUF },
	{ "file", MODE_FILE },
	{ "sysinfo", MODE_SYSINFO },
	{ "all", MODE_ALL },
};

static void usage(const char *name)
{
	fprintf(stderr,
		"usage: %s [hello|anon|buf|file|sysinfo|all]\n",
		name);
}

static enum mode parse(const char *name)
{
	size_t i;

	if (name == NULL)
		return MODE_INVALID;

	for (i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
		if (strcmp(name, modes[i].name) == 0)
			return modes[i].id;
	}

	return MODE_INVALID;
}

static int sysinfo(void)
{
	long pagesize = sysconf(_SC_PAGESIZE);

	if (pagesize <= 0)
		return -1;

	if (printf("sysinfo: page_size=%ld bytes, pointer_size=%zu bytes\n",
		   pagesize, sizeof(void *)) < 0)
		return -1;

	return 0;
}

int main(int argc, char **argv)
{
	const char *name = (argc > 1) ? argv[1] : "all";
	const long  left = 0x0003;
	const long  right = 0x0004;
	enum mode mode = parse(name);
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
			   left, right, _add(left, right)) < 0)
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
