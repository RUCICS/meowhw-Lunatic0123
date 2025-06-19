#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <stdlib.h>
long io_blocksize(){
    long page_size = sysconf(_SC_PAGESIZE);
    return page_size;
}
int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Please enter the file you want to cat\n");
        fflush(stdout);
        return 1;
    }
    
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "file open failed\n");
        close(fd);
        return 1;
    }
    long size = io_blocksize();
    char* buffer = malloc(size);
    ssize_t byte_read;
    while((byte_read = read(fd, buffer, size)) > 0){
        if (write(STDOUT_FILENO, buffer, byte_read) == -1) {
            perror("error writing to stdout");
            close(fd); 
            return 1;
        }
    }
    free(buffer);
    close(fd);
    return 0;
}