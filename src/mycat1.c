#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
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
    char buffer[1];
    ssize_t byte_read;
    while((byte_read = read(fd, buffer, 1)) > 0){
        printf("%s", buffer);
    }
}