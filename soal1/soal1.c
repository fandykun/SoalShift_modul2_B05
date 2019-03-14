#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main()
{
    pid_t pid, sid;

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

    if ((chdir("/home/fandykun/modul2/soal1/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        struct stat st = {0};
        if(stat("gambar", &st) == -1)
            mkdir("gambar", 0775);
        
        struct dirent *dir;
        DIR *d;
        d = opendir("/home/fandykun/modul2/soal1/");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                int index = 0;
                char old_name[256], src[256];

                snprintf(old_name, 256, "%s", dir->d_name);
                for (int i = 0; i < sizeof(dir->d_name); i++)
                {
                    if (dir->d_name[i] == '.')
                    {
                        index = i;
                        break;
                    }
                }
                if (sizeof(old_name) >= index + 3)
                {
                    if (old_name[index + 1] == 'p' && old_name[index + 2] == 'n' && old_name[index + 3] == 'g')
                    {
                        old_name[index] = '\0';
                        snprintf(src, 256, "gambar/%s_grey.png", old_name);
                        rename(dir->d_name, src);
                    }
                }
            }
            closedir(d);
        }

        sleep(10);
    }
    exit(EXIT_SUCCESS);
}