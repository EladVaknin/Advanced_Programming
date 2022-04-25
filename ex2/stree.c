
#define _XOPEN_SOURCE 600 // Get nftw() 
#include <ftw.h>
#include <sys/types.h>  // Type definitions used by many programs 
#include <stdio.h>	    // Standard I/O functions 
#include <stdlib.h>	    // Prototypes of commonly used library functions, plus EXIT_SUCCESS and EXIT_FAILURE constants                            
#include <unistd.h>	    // Prototypes for many system calls 
#include <errno.h>	    // Declares errno and defines error constants 
#include <string.h>	    // Commonly used string-handling functions 
#include <pwd.h>	    // Contains users names 
#include <grp.h>	    // Contains groups names 

char *path;
int f_counter = 0;    //file counter
int d_file = 0;       // directory counter



static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb){          // Callback function called by ftw() 
	printf(" %*s", 3 * ftwb->level, " ");    // Indent suitably
	if (strcmp(pathname, path) != 0)    //if not start directory
	{
		printf("%s%s", "├──", "[ ");
		if (type == FTW_NS) // the file not working
		{ 
			printf("?");
		}
		else
		{
			// file type - print 
			switch (sbuf->st_mode & S_IFMT)
			{ 
			case S_IFREG:
				f_counter++;
				printf("-");
				break;
			case S_IFDIR:
				d_file++;
				printf("d");
				break;
			case S_IFCHR:
				printf("c");
				break;
			case S_IFBLK:
				printf("b");
				break;
			case S_IFLNK:
				printf("l");
				break;
			case S_IFIFO:
				printf("p");
				break;
			case S_IFSOCK:
				printf("s");
				break;
			default:
				printf("?");
				break; // Should never happen (on Linux) 
			}

			//OWNER PERMISSION
			if (sbuf->st_mode & S_IRUSR) // read 
			{ 
				printf("r");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IWUSR) //wirte
			{ 
				printf("w");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IXUSR) //excute
			{ 
				printf("x");
			}
			else
			{
				printf("-");
			}

			//GROUP PERMISSIONES
			if (sbuf->st_mode & S_IRGRP) //read
			{ 
				printf("r");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IWGRP) // wirte
			{ 
				printf("w");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IXGRP) //excute
			{ 
				printf("x");
			}
			else
			{
				printf("-");
			}


			//OTHER PERMISSIONES
			if (sbuf->st_mode & S_IROTH)  //read
			{ 
				printf("r");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IWOTH)  //wirte
			{ 
				printf("w");
			}
			else
			{
				printf("-");
			}
			if (sbuf->st_mode & S_IXOTH) //excute
			{ 
				printf("x");
			}
			else
			{
				printf("-");
			}


			//USER name
			struct passwd *pwd;
			pwd = getpwuid(sbuf->st_uid);
			if (pwd != NULL)
			{
				printf("           %s", pwd->pw_name); //user name by user id
			}
			else
			{
				printf("           %d", sbuf->st_uid); //print id
			}



			//GROUP name
			struct group *grp;
			grp = getgrgid(sbuf->st_gid);
			if (grp != NULL)
			{
				printf("             %s", grp->gr_name); //group name by group id
			}
			else
			{
				printf("             %d", sbuf->st_gid); //print id
			}

			//FILE SIZE
			printf("             %ld", sbuf->st_size);
		}

		printf("%s", "] ");
		printf("%s\n", &pathname[ftwb->base]); // basename
	}
	else
	{
		printf("  %s\n", &pathname[ftwb->base]);
	}
	return 0; 
}

  

  //MAIN 
int main(int argc, char *argv[]){
	int flags = 0;
	// // check for - if file exist
	// if (access(argv[1], F_OK) == 0)
	// {
	// 	nftw(argv[1], dirTree, 20, flags);
	// }
	// else
	// {
	// 	fprintf(stderr, "ERROR: directory does not exist\n");
	// 	exit(-1);
	// }

	if (argc == 1)   //if didn't got any parameters, start from current directory
	{ 
		path = (char *)malloc(strlen(".") + 1);
		strcpy(path, ".");
		if (nftw(path, dirTree, 10, flags) == -1)
		{
			free(path);
			perror("nftw");
			exit(EXIT_FAILURE);
		}
	}
	else if (argc > 2)
	{
		fprintf(stderr, "Usage: %s directory-path\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		path = (char *)malloc(strlen(".") + 1);
		strcpy(path, argv[1]);
		if (nftw(path, dirTree, 10, flags) == -1)
		{
			free(path);
			perror("nftw");
			exit(EXIT_FAILURE);
		}
	}
	free(path);
	printf("%d directory, %d files\n", d_file, f_counter);
	exit(EXIT_SUCCESS);
}
