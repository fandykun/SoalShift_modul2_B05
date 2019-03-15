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
    int log_inc = 1;
    int run_time = 0;

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

    if ((chdir("/home/fandykun/modul2/soal5/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    FILE *fpid;
    fpid = fopen("inipidnya.txt", "w");
    fprintf(fpid, "%d", sid);
    fclose(fpid);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        FILE *flog, *fdst;
        time_t t = time(NULL);
        struct tm time = *localtime(&t);
        char date[100];
        char directory[100];
        char log_name[100];

        snprintf(date, 100, "%02d:%02d:%04d-%02d:%02d", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900, time.tm_hour, time.tm_min);
        if(log_inc == 1)
            snprintf(directory, 100, "/home/fandykun/modul2/%s", date);
        
        struct stat st = {0};
        if(run_time >= 1800){
            snprintf(directory, 100, "/home/fandykun/modul2/%s", date);
            run_time = 0;
        }

        if (stat(directory, &st) == -1 && chdir(directory) < 0){
            mkdir(directory, 0775);
            chdir(directory);
        }

        snprintf(log_name, 100, "%s/log%d.log", directory, log_inc++);

        char readlog[255];
        flog = fopen("/var/log/syslog", "r");
        fdst = fopen(log_name, "w");

        if (flog != NULL)
        {
            while (fgets(readlog, 255, flog) != NULL)
                fputs(readlog, fdst);
        }
        fclose(flog);
        fclose(fdst);

        run_time +=60;
        sleep(60);
    }

    exit(EXIT_SUCCESS);
}