#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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
    pid = fork();

    // If pid < 0, fork failed
    if (pid < 0)
    {
	exit(EXIT_FAILURE);
    }

    /* 
    If pid > 0, it's the parent.
    In order to make a daemon,
    terminate the parent process
    */
    if (pid > 0)
    {
	exit(EXIT_SUCCESS);
    }

    /*
    Set file mode so it can be 
    written and read properly
    */
    umask(0);

    
    // Create an unique SID
    sid = setsid();
    if (sid < 0)
    {
	exit(EXIT_FAILURE);
    }

    /*
    Change directory
    Must be an active and
    always exists directory
    */
    if ((chdir("/home/abraham")) < 0)
    {
	exit(EXIT_FAILURE);
    }

    
    // Close the standard file descriptor
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Main program
    
    int count = 1;
    while(1)
    {
	// Check if makan_enak.txt has been recently accessed
	// Call a function stat() to return information about 
	// a file
	struct stat attrib;
	stat("Documents/makanan/makan_enak.txt", &attrib);
	    
	// attrib will check the attribute / information of
	// makan_enak.txt, if makan_enak.txt is being accessed,
	// then record the time with variable enak
	time_t enak = attrib.st_atime;
	    
	// Check what time is it now with time()
	time_t sehat = time(NULL);
	    
	// If makan_enak.txt is being accessed in a certain
	// 30 seconds, then make a new file named makan_sehat#.txt
	if(difftime(sehat, enak) <= 30)
	{		
	    char num[10];
	    sprintf(num, "%d", count);

	    char filename[1000] = "/home/abraham/Documents/makanan/makan_sehat";
	    char ext[] = ".txt";

	    strcat(filename, num);
	    strcat(filename, ext);
	    FILE *fPtr = fopen(filename, "w");

	    fprintf(fPtr, "Kalau diet yang bener\n");
	    fclose(fPtr);
		
	    count++;
	}

	sleep(5);
    }

    exit(EXIT_SUCCESS);

}
