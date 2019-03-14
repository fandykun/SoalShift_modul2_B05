#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main()
{
    pid_t pid, sid;
    FILE *fp;
    double now = time(NULL);
    double elapsed = time(NULL);

    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        elapsed = (double)time(NULL);

        char filename[80];
        snprintf(filename, 80, "/home/fandykun/modul2/temp.txt");
        fp = fopen(filename, "a");

        if(elapsed - now >= 30){
            now = elapsed;
            fprintf(fp, "masuk\n");  
        }
        fprintf(fp, "now:%.4lf elapsed:%.4lf\n", now, elapsed);
        printf("%lf : %lf", elapsed, now);
        fclose(fp);
        sleep(5);
    }

    exit(EXIT_SUCCESS);
}