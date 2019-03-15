#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
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
    kill(pid, SIGTERM);
    return 0;
}