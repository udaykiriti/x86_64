/*
 * map.c - mmap demonstration: anonymous, typed buffer, and file-backed mappings
 *
 * Each case shows a distinct use of the mmap(2) interface on Linux x86_64.
 * Error handling follows the negative-errno convention: helpers return
 * a negative errno value on failure; callers print via strerror(-ret).
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * writeall - write exactly len bytes to fd, retrying on EINTR.
 *
 * Returns 0 on success, negative errno on failure.
 */
static int writeall(int fd, const void *buf, size_t len)
{
	const char *p = buf;

	while (len > 0) {
		ssize_t n = write(fd, p, len);

		if (n < 0) {
			if (errno == EINTR)
				continue;
			return -errno;
		}
		if (n == 0)
			return -EIO;

		p   += (size_t)n;
		len -= (size_t)n;
	}

	return 0;
}

/*
 * anon - Case 1: Anonymous mapping
 *
 * Allocates one page with MAP_ANONYMOUS, copies a string into it,
 * writes it to stdout via writeall, then unmaps the page.
 *
 * Returns 0 on success, negative errno on failure.
 */
int anon(void)
{
	const long   pgl    = sysconf(_SC_PAGESIZE);
	const char  *msg    = "map case 1: anonymous page text\n";
	const size_t msglen = strlen(msg);
	size_t       pgsz;
	char        *buf;
	int          ret;

	if (pgl <= 0)
		return -EINVAL;

	pgsz = (size_t)pgl;

	buf = mmap(NULL, pgsz, PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (buf == MAP_FAILED)
		return -errno;

	memcpy(buf, msg, msglen);
	ret = writeall(STDOUT_FILENO, buf, msglen);

	if (munmap(buf, pgsz) != 0 && ret == 0)
		ret = -errno;

	return ret;
}

/*
 * intbuf - Case 2: Anonymous mapping as a typed integer buffer
 *
 * Allocates an array of ints via MAP_ANONYMOUS, fills each element
 * with its index squared, prints the results, then unmaps the region.
 *
 * Returns 0 on success, negative errno on failure.
 */
int intbuf(void)
{
	const size_t count = 16;
	const size_t size  = count * sizeof(int);
	int         *arr;
	size_t       i;
	int          ret = 0;

	arr = mmap(NULL, size, PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (arr == MAP_FAILED)
		return -errno;

	for (i = 0; i < count; i++)
		arr[i] = (int)(i * i);

	if (printf("map case 2: int buffer ->") < 0) {
		ret = -EIO;
		goto out;
	}

	for (i = 0; i < count; i++) {
		if (printf(" %d", arr[i]) < 0) {
			ret = -EIO;
			goto out;
		}
	}

	if (putchar('\n') == EOF)
		ret = -EIO;

out:
	if (munmap(arr, size) != 0 && ret == 0)
		ret = -errno;

	return ret;
}

/*
 * filemap - Case 3: File-backed shared mapping
 *
 * Creates a file, maps it MAP_SHARED, writes through the mapping,
 * calls msync(2) to flush to disk, unmaps it, then reads back via
 * read(2) to confirm the data reached the file.
 *
 * Returns 0 on success, negative errno on failure.
 */
int filemap(void)
{
	const char  *path   = "obj/map.bin";
	const char  *msg    = "map case 3: file-backed mapping";
	const size_t mapsize = 32;
	const size_t msglen = strlen(msg);
	char         rbuf[64];
	char        *mem;
	ssize_t      rd;
	int          fd;
	int          ret    = 0;

	fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		return -errno;

	if (ftruncate(fd, (off_t)mapsize) != 0) {
		ret = -errno;
		goto ofd;
	}

	mem = mmap(NULL, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == MAP_FAILED) {
		ret = -errno;
		goto ofd;
	}

	memset(mem, 0, mapsize);
	memcpy(mem, msg, msglen < mapsize ? msglen : mapsize - 1);

	if (msync(mem, mapsize, MS_SYNC) != 0) {
		ret = -errno;
		goto omap;
	}

	if (munmap(mem, mapsize) != 0) {
		ret = -errno;
		goto ofd;
	}
	mem = NULL; /* prevent double-unmap in omap */

	if (lseek(fd, 0, SEEK_SET) < 0) {
		ret = -errno;
		goto ofd;
	}

	memset(rbuf, 0, sizeof(rbuf));
	rd = read(fd, rbuf, mapsize);
	if (rd < 0) {
		ret = -errno;
		goto ofd;
	}
	if ((size_t)rd != mapsize) {
		ret = -EIO;
		goto ofd;
	}
	if (memcmp(rbuf, msg, msglen) != 0) {
		ret = -EIO;
		goto ofd;
	}

	if (printf("map case 3: file says -> %s\n", rbuf) < 0)
		ret = -EIO;

omap:
	if (mem != NULL && munmap(mem, mapsize) != 0 && ret == 0)
		ret = -errno;
ofd:
	if (close(fd) != 0 && ret == 0)
		ret = -errno;

	return ret;
}
