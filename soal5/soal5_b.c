#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;
    FILE *fpid;
    if ((fpid = fopen("inipidnya.txt", "r")) == NULL)
    {
        printf("yah, revisi dong\n");
        return 0;
    }

    fscanf(fpid, "%d", &pid);

    char *argv[3] = {"kill", pid, NULL};

    execv("/bin/kill", argv);

    return 0;
}