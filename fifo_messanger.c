#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define MSize 1000000

void WritingFifo (char* fifo) {
    int f = open(fifo, O_WRONLY);

    if (f < 0) {
        printf("Can\'t open for reading");
        exit (-1);
    }

    char message[MSize];
    memset(message, '0', MSize);

    while(1) {
        fgets(message, MSize, stdin);
        write(f, message, MSize);
        memset(message, '0', MSize);
    }

}

void ReadingFifo (char* fifo) {
    int f = open(fifo, O_RDONLY);

    if (f < 0) {
        printf("Can\'t open for reading");
        exit (-1);
    }

    char message[MSize];
    memset(message, '0', MSize);

    while(1) {
        int size = read(f, message, MSize);
        if (size <= 0) {
            printf("your pen pal left the chat\n");
            close(f);
            exit(0);
        }
        printf("%s", message);
        memset(message, '0', MSize);
    }

}

int main(int argc, char** argv) {
    int fRead = 0, fWrite = 0;
    (void)umask(0);

    if((fRead = mknod("r", S_IFIFO | 0666, 0)) < 0){
        printf("Can\'t create reading FIFO\n");
        exit(-1);
    }

    if((fWrite = mknod("w", S_IFIFO | 0666, 0)) < 0){
        printf("Can\'t writing create FIFO\n");
        exit(-1);
    }

    char fifoRead[2], fifoWrite[2];
    memset(fifoRead, '0', 2);
    memset(fifoWrite, '0', 2);

    int mode = atoi(argv[1]);

    if (mode == 0) {
        fifoRead[0] = 'r';
        fifoWrite[0] = 'w';
    } else {
        fifoRead[0] = 'w';
        fifoWrite[0] = 'r';
    }

    int pid = fork();

    if (pid == 0) {
        WritingFifo(fifoWrite);
    } else {
        ReadingFifo(fifoRead);
    }

    return 0;
}