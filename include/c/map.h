#ifndef MAP_H
#define MAP_H

/*
 * mmap demo helpers.
 *
 * Each function returns 0 on success or a negative errno value on failure.
 */
int anon(void);
int intbuf(void);
int filemap(void);

#endif /* MAP_H */
