#include <stdio.h>
#include <sys/mman.h>   // mmap
#include <unistd.h>     // getpagesize
#include <string.h>

int main() {
    size_t size = getpagesize();  // usually 4096 bytes

    // Req mem from kernel
    void *mem = mmap(
        /*we hope and pray for that */
        NULL,                   // let kernel choose address
        size,                   // sz
        PROT_READ | PROT_WRITE, // permissions
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,                     // no file
        0
    );

    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    printf("Allocated memory at: %p\n", mem);
    
    // wr into mapped memory
    strcpy((char*)mem, "Hello from mmap!");

    printf("Data in memory: %s\n", (char*)mem);

    // free(memory)
    munmap(mem, size);

    return 0;
}
