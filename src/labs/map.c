#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int write_all(int fd, const void *buf, size_t len)
{
	const char *p = buf;

	while (len > 0) {
		ssize_t n = write(fd, p, len);

		if (n < 0) {
			if (errno == EINTR)
				continue;
			return -errno;
		}

		p += (size_t)n;
		len -= (size_t)n;
	}

	return 0;
}

/*
 * Topic: Anonymous mapping
 * Goal : Allocate one page, write text into it, print it, unmap it.
 */
static int anon_text(void)
{
	const long page_size_l = sysconf(_SC_PAGESIZE);
	const char *msg = "map case 1: anonymous page text\n";
	size_t page_size;
	char *buf;
	int ret = 0;

	if (page_size_l <= 0)
		return -EINVAL;

	page_size = (size_t)page_size_l;

	buf = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (buf == MAP_FAILED)
		return -errno;

	memcpy(buf, msg, strlen(msg));
	ret = write_all(STDOUT_FILENO, buf, strlen(msg));
	if (ret != 0)
		goto out;

out:
	if (munmap(buf, page_size) != 0 && ret == 0)
		ret = -errno;
	return ret;
}

/*
 * Topic: Anonymous mapping as a typed buffer
 * Goal : Use mmap memory as int array and print computed values.
 */
static int int_buffer(void)
{
	const size_t n = 16;
	size_t i;
	int *arr;
	int ret = 0;

	arr = mmap(NULL, n * sizeof(*arr), PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (arr == MAP_FAILED)
		return -errno;

	for (i = 0; i < n; i++)
		arr[i] = (int)(i * i);

	if (printf("map case 2: int buffer -> ") < 0) {
		ret = -EIO;
		goto out;
	}

	for (i = 0; i < n; i++)
		if (printf("%d%s", arr[i], (i + 1 == n) ? "\n" : " ") < 0) {
			ret = -EIO;
			goto out;
		}

out:
	if (munmap(arr, n * sizeof(*arr)) != 0 && ret == 0)
		ret = -errno;
	return ret;
}

/*
 * Topic: File-backed mapping
 * Goal : Map a file, write through memory, sync, and verify content.
 */
static int file_backed(void)
{
	const char *path = "obj/map.bin";
	const char *msg = "map case 3: file-backed mapping";
	const size_t len = 32;
	int fd = -1;
	char *mem;
	char out[64];
	ssize_t rd;
	int ret = 0;

	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		return -errno;

	if (ftruncate(fd, (off_t)len) != 0) {
		ret = -errno;
		goto out_fd;
	}

	mem = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == MAP_FAILED) {
		ret = -errno;
		goto out_fd;
	}

	memset(mem, 0, len);
	memcpy(mem, msg, strlen(msg) < len ? strlen(msg) : len - 1);

	if (msync(mem, len, MS_SYNC) != 0) {
		ret = -errno;
		goto out_map;
	}

	if (munmap(mem, len) != 0) {
		ret = -errno;
		goto out_fd;
	}

	if (lseek(fd, 0, SEEK_SET) < 0) {
		ret = -errno;
		goto out_fd;
	}

	memset(out, 0, sizeof(out));
	rd = read(fd, out, len);
	if (rd < 0)
		ret = -errno;

	if (printf("map case 3: file says -> %s\n", out) < 0 && ret == 0)
		ret = -EIO;

out_fd:
	if (fd >= 0 && close(fd) != 0 && ret == 0)
		ret = -errno;
	return ret;

out_map:
	if (munmap(mem, len) != 0 && ret == 0)
		ret = -errno;
	goto out_fd;
}

int main(int argc, char **argv)
{
	int ret;
	const char *mode = (argc > 1) ? argv[1] : "all";

	if (strcmp(mode, "anon") == 0 || strcmp(mode, "all") == 0) {
		ret = anon_text();
		if (ret != 0) {
			fprintf(stderr, "anon_text failed: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (strcmp(mode, "buf") == 0 || strcmp(mode, "all") == 0) {
		ret = int_buffer();
		if (ret != 0) {
			fprintf(stderr, "int_buffer failed: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (strcmp(mode, "file") == 0 || strcmp(mode, "all") == 0) {
		ret = file_backed();
		if (ret != 0) {
			fprintf(stderr, "file_backed failed: %s\n", strerror(-ret));
			return 1;
		}
	}

	if (strcmp(mode, "anon") != 0 &&
	    strcmp(mode, "buf") != 0 &&
	    strcmp(mode, "file") != 0 &&
	    strcmp(mode, "all") != 0) {
		fprintf(stderr, "usage: %s [anon|buf|file|all]\n", argv[0]);
		return 1;
	}

	return 0;
}
