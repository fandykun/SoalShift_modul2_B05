#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int pid;
int _pipe[2];

int main()
{
    int status;

    if(pipe(_pipe) == -1) {
        printf("error pipe1");
        _exit(1);
    }

    // fork (unzip)
    if ( (pid = fork()) == -1) {
        printf("error fork1");
        _exit(1);
    }
    else if(pid == 0) {
        char *argv[3] = {"touch", "daftar.txt", NULL};
        execv("/usr/bin/touch", argv);
        // this is child
    }
    //parent

    // fork create file
    if( (pid = fork()) == -1){
        printf("error fork2");
        _exit(1);
    }
    else if(pid == 0){
        char *argv[3] = {"unzip", "campur2.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }

    while( (wait(&status)) > 0) ;

    if ( (pid = fork()) == -1) {
        printf("error fork3");
        _exit(1);
    }
    else if(pid == 0){
        dup2(_pipe[1], 1);

        close(_pipe[0]);
        close(_pipe[1]);

        execlp("/bin/ls", "ls", "campur2", NULL);
        
        _exit(1);
    }
    //parent
    else {
        while( (wait(&status)) > 0) ;
        dup2(_pipe[0], 0);

        close(_pipe[0]);
        close(_pipe[1]);
        int file = open("daftar.txt", O_APPEND | O_WRONLY);
        if(file < 0) return 1;

        if(dup2(file, 1) < 0){
            close(file);
            return 1;
        }
        execlp("/bin/grep", "grep", "\\.txt$", NULL);
        close(file);
        
        _exit(1);
    }

}