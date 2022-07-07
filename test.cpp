#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

static const int BLOCK_SIZE = 512;
static const int ALIGNMENT = 512;
static const int OFFSET = 1024 * ALIGNMENT; // 1024 is arbitrary


int main (int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "usage: %s device\n", argv[0]);
        return 1;
    }

    int d = open(argv[1], O_RDWR | O_DIRECT | O_SYNC);
    if (d == -1) {
        perror(argv[1]);
        return 1;
    }

    char *block[2], *buffer;
    int index = 0, count = -1;

    // buffers must be aligned for O_DIRECT.
    posix_memalign((void **)&(block[0]), ALIGNMENT, BLOCK_SIZE);
    posix_memalign((void **)&(block[1]), ALIGNMENT, BLOCK_SIZE);
    posix_memalign((void **)&buffer, ALIGNMENT, BLOCK_SIZE);

    // different contents in each buffer
    memset(block[0], 0x55, BLOCK_SIZE);
    memset(block[1], 0xAA, BLOCK_SIZE);

    while (true) {

        // alternate buffers
        index = 1 - index;

        if (!((++ count) % 100)) {
            printf("%i\n", count);
            fflush(stdout);
        }

        // write -> sync -> read back -> compare
        if (lseek(d, OFFSET, SEEK_SET) == (off_t)-1)
            perror("lseek(w)");
        else if (write(d, block[index], BLOCK_SIZE) != BLOCK_SIZE)
            perror("write");
        else if (fsync(d))
            perror("fsync");
        else if (lseek(d, OFFSET, SEEK_SET) == (off_t)-1)
            perror("lseek(r)");
        else if (read(d, buffer, BLOCK_SIZE) != BLOCK_SIZE)
            perror("read");
        else if (memcmp(block[index], buffer, BLOCK_SIZE))
            fprintf(stderr, "memcmp: test failed\n");
        else
            continue;

        printf("failed after %i successful cycles.\n", count);
        break;

    }

}
