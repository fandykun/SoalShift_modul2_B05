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

#include <pwd.h>
#include <grp.h>

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
    uid_t userID;
    gid_t groupID;
    
    while(1)
    {
	struct stat attrib;
	struct group *grup;
	struct passwd *pwd;
	stat("hatiku/elen.ku", &attrib);
	
	chmod("hatiku/elen.ku", 777);
	
	userID = attrib.st_uid;
	groupID = attrib.st_gid;
	
	grup = getgrgid(groupID);
	pwd = getpwuid(userID);
	
	if (!(strcmp(pwd->pw_name, "www-data")) && !(strcmp(grup->gr_name, "www-data")))
	{	    
	    remove("hatiku/elen.ku");
	}
	sleep(3);
    }

    exit(EXIT_SUCCESS);

}
