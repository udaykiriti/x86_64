/*
 * Program: main
 * Model  : C driver for x86_64 ASM routines
 * Goal   : run assembly hello and mmap demos
 *
 * Usage  : ./bin/program [hello|anon|buf|file|cow|shared|simd|sysinfo|all]
 *
 *   hello   - run ASM hello routine
 *   anon    - anonymous page mapping
 *   buf     - anonymous integer buffer
 *   file    - file-backed shared mapping
 *   cow     - private file mapping that demonstrates copy-on-write
 *   shared  - shared anonymous mapping visible across fork
 *   simd    - add two integer vectors in ASM with SSE2
 *   sysinfo - show basic runtime system information
 *   all     - run all in order (default)
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "map.h"

extern void _hello(void);
extern long _add(long a, long b);
extern void vecadd4(const int *a, const int *b, int *c);
extern int bitwise_and(int a, int b);
extern int bitwise_or(int a, int b);
extern int bitwise_xor(int a, int b);

enum mode {
	MODE_INVALID = 0,
	MODE_HELLO,
	MODE_ANON,
	MODE_BUF,
	MODE_FILE,
	MODE_COW,
	MODE_SHARED,
	MODE_SIMD,
	MODE_BITWISE,
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
	{ "cow", MODE_COW },
	{ "shared", MODE_SHARED },
	{ "simd", MODE_SIMD },
	{ "bitwise", MODE_BITWISE },
	{ "sysinfo", MODE_SYSINFO },
	{ "all", MODE_ALL },
};

static void usage(const char *name)
{
	fprintf(stderr,
		"usage: %s [hello|anon|buf|file|cow|shared|simd|bitwise|sysinfo|all]\n",
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
	if (sysconf(_SC_PAGESIZE) <= 0)
		return -1;

	if (printf("sysinfo: page_size=%ld bytes, pointer_size=%zu bytes\n",
		   sysconf(_SC_PAGESIZE), sizeof(void *)) < 0)
		return -1;

	return 0;
}

static int simd(void)
{
	const int a[4] = { 1, 2, 3, 4 };
	const int b[4] = { 10, 20, 30, 40 };
	int c[4];
	size_t i;

	vecadd4(a, b, c);

	if (printf("simd: vector add ->") < 0)
		return -1;

	for (i = 0; i < sizeof(c) / sizeof(c[0]); i++) {
		if (printf(" %d", c[i]) < 0)
			return -1;
	}

	if (putchar('\n') == EOF)
		return -1;

	return 0;
}

static int bitwise(void)
{
	const int x = 0xAA;
	const int y = 0x55;

	if (printf("bitwise: AND(0x%02X, 0x%02X) = 0x%02X\n",
		   x, y, bitwise_and(x, y)) < 0)
		return -1;

	if (printf("bitwise: OR(0x%02X, 0x%02X) = 0x%02X\n",
		   x, y, bitwise_or(x, y)) < 0)
		return -1;

	if (printf("bitwise: XOR(0x%02X, 0x%02X) = 0x%02X\n",
		   x, y, bitwise_xor(x, y)) < 0)
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

	if (argc > 2) 
  {
		usage(argv[0]);
		return 1;
	}

	if (mode == MODE_INVALID) 
  {
		usage(argv[0]);
		return 1;
	}

	if (mode == MODE_HELLO || mode == MODE_ALL) 
  {
		_hello();
		if (printf("_add(%ld, %ld) = %ld\n",
			   left, right, _add(left, right)) < 0 ||
		    fflush(stdout) != 0)
			return 1;
	}

	if (mode == MODE_ANON || mode == MODE_ALL) 
  {
		ret = anon();

		if (ret != 0) 
    {
			fprintf(stderr, "anon: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_BUF || mode == MODE_ALL) 
  {
		ret = intbuf();
		if (ret != 0) 
    {
			fprintf(stderr, "intbuf: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_FILE || mode == MODE_ALL) 
  {
		ret = filemap();
		if (ret != 0) 
    {
			fprintf(stderr, "filemap: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_COW || mode == MODE_ALL) {
		ret = cowmap();
		if (ret != 0) 
    {
			fprintf(stderr, "cowmap: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_SHARED || mode == MODE_ALL) {
		ret = sharedmap();
		if (ret != 0) 
    {
			fprintf(stderr, "sharedmap: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (mode == MODE_SIMD || mode == MODE_ALL) {
		ret = simd();
		if (ret != 0) 
    {
			fprintf(stderr, "simd: failed to run vector demo\n");
			return 1;
		}
	}

	if (mode == MODE_BITWISE || mode == MODE_ALL) {
		ret = bitwise();
		if (ret != 0) 
    {
			fprintf(stderr, "bitwise: failed to run bitwise demo\n");
			return 1;
		}
	}

	if (mode == MODE_SYSINFO || mode == MODE_ALL) {
		ret = sysinfo();
		if (ret != 0) 
    {
			fprintf(stderr, "sysinfo: failed to read system info\n");
			return 1;
		}
	}

	return 0;
}
