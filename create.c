#include "mytest.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_INTEGERS (1 << 20)

int create_source_data() { 
    int i;
    int tmp = 1;
    int fd[4];
    int cfd[4];

    fd[0] = open("p1.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    fd[1] = open("p2.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    fd[2] = open("p3.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    fd[3] = open("p4.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);

    for (i = 1; i <= TOTAL_INTEGERS; i++) {
        int fileIndex = (i - 1) % 4;
        write(fd[fileIndex], &i, sizeof(int));
    }

    cfd[0] = open("CHUNK1", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    cfd[1] = open("CHUNK2", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    cfd[2] = open("CHUNK3", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    cfd[3] = open("CHUNK4", O_CREAT | O_TRUNC | O_WRONLY, 0644);

    /*lseek()의 offset 접근을 위한 chunk 파일 사전 생성*/
    for (i = 1; i <= TOTAL_INTEGERS / (1 << 10); i++)
    {
        write(cfd[0], &tmp, sizeof(int));
        write(cfd[1], &tmp, sizeof(int));
        write(cfd[2], &tmp, sizeof(int));
        write(cfd[3], &tmp, sizeof(int));
    }

    for (i = 0; i < 4; i++) {
        close(fd[i]);
    }

    for (i = 0; i < 4; i++) {
        close(cfd[i]);
    }

    printf("**Distribute input data across processes.\n");
    return 0;
}