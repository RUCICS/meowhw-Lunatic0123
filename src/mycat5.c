#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/statfs.h>
#include <stdint.h>
#include <fcntl.h>
char* origin_ptr;
long io_blocksize(char* filename, int times){
    long size = 0;
    long page_size = sysconf(_SC_PAGESIZE);
    struct statfs buf;
    if (statfs(filename, &buf) == -1) {
        perror("Error getting filesystem info");
        return -1;
    }
    if(buf.f_bsize % 2 == 0){
        size = buf.f_bsize;
    }
    return size * times;
}
char* align_alloc(size_t size) {
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t total_size = size + page_size - 1;
    char* ptr = (char*)malloc(total_size);
    origin_ptr = ptr;
    if (ptr == NULL) return NULL;
    char* aligned_ptr = (char*)(((unsigned long long)(ptr + page_size - 1)) & ~(page_size - 1));
    return aligned_ptr;
}

void align_free(void* ptr) {
    free(ptr);
}
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Please enter the file you want to cat\n");
        fflush(stdout);
        return 1;
    }
    struct statfs buf;
    if (statfs(argv[1], &buf) == -1) {
        perror("Error getting filesystem info");
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "file open failed\n");
        close(fd);
        return 1;
    }
    
    long size = io_blocksize(argv[1], atoi(argv[2]));
    if(size == -1){
        perror("Error getting size");
        return 1;
    }
    char* buffer = align_alloc(size);
    ssize_t byte_read;
    while((byte_read = read(fd, buffer, size)) > 0){
        if (write(STDOUT_FILENO, buffer, byte_read) == -1) {
            perror("error writing to stdout");
            close(fd); 
            return 1;
        }
    }
    align_free(origin_ptr);
    close(fd);
    return 0;
}