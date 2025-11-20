#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int p1[2], p2[2], p3[2], p4[2];
    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);
    int start = uptime();
    int duration = 100;

    int pid1 = fork();
    if(pid1 == 0) {
        close(p1[0]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);
        close(p4[0]); close(p4[1]);

        int counter = 0;

        while(uptime() - start < duration)
            counter++;
        
        write(p1[1], &counter, sizeof(counter));
        close(p1[1]);
        exit(0);
    }

    int pid2 = fork();
    if(pid2 == 0) {
        close(p2[0]);
        close(p1[0]); close(p1[1]);
        close(p3[0]); close(p3[1]);
        close(p4[0]); close(p4[1]);

        int counter = 0;

        while(uptime() - start < duration)
            counter++;

        write(p2[1], &counter, sizeof(counter));
        close(p2[1]);
        exit(0);
    }

    int pid3 = fork();
    if(pid3 == 0) {
        close(p3[0]);
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p4[0]); close(p4[1]);

        int counter = 0;

        while(uptime() - start < duration)
            counter++;

        write(p3[1], &counter, sizeof(counter));
        close(p3[1]);
        exit(0);
    }

    int pid4 = fork();
    if(pid4 == 0) {
        close(p4[0]);
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        int counter = 0;
        
        while(uptime() - start < duration)
            counter++;

        write(p4[1], &counter, sizeof(counter));
        close(p4[1]);
        exit(0);
    }

    close(p1[1]);
    close(p2[1]);
    close(p3[1]);
    close(p4[1]);

    setprocshare(pid1, 1);
    setprocshare(pid2, 5);
    setprocshare(pid3, 10);
    setprocshare(pid4, 20);

    int c1, c2, c3, c4;

    read(p1[0], &c1, sizeof(c1));
    read(p2[0], &c2, sizeof(c2));
    read(p3[0], &c3, sizeof(c3));
    read(p4[0], &c4, sizeof(c4));

    wait(0);
    wait(0);
    wait(0);
    wait(0);

    close(p1[0]);
    close(p2[0]);
    close(p3[0]);
    close(p4[0]);

    printf("c1 = %d, c2 = %d, c3 = %d, c4 = %d\n", c1, c2, c3, c4);
    exit(0);
}