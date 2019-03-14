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

    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        FILE *file_txt;
        struct dirent *dir;
        DIR *d;
        d = opendir("/home/fandykun/modul2/");
        char filename[80];
        snprintf(filename, 80, "/home/fandykun/modul2/temp.txt");
        file_txt = fopen(filename, "a");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                int index = 0;
                char old_name[256], src[256];

                // fprintf(file_txt, "%s\n", dir->d_name);
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

                        fprintf(file_txt, "%s : %s : %s\n", old_name, dir->d_name, src);
                        rename(dir->d_name, src);
                        // execv("/bin/mv", test);
                    }
                }
            }
            closedir(d);
        }
        fclose(file_txt);
        sleep(5);
    }
    exit(EXIT_SUCCESS);
}