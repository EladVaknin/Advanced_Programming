#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SHOWHOST
#define SHOWLINE

void showtime(long);
void showtime1(long);
void show_info(struct utmp*, int size);

int wtmpfd; //read from here
const char* SYSTEM_BOOT = "system boot";
int counter = 0;
int numberOfRecords;

int main(int argc, char const* argv[])
{
    struct utmp wtbuf;      // buffer to catch the historical utmp (linux system that track for login/out frpm the system)
    numberOfRecords = atoi(argv[1]);
    int len = sizeof(struct utmp);

    if ((wtmpfd = open(WTMP_FILE, O_RDONLY)) == -1) {
        perror(WTMP_FILE);
        exit(1);
    }

    if (read(wtmpfd, &wtbuf, len) > len) {
        perror(UTMP_FILE);
        exit(1);
    }
    while (counter < numberOfRecords) {
        counter++;
        lseek(wtmpfd, -counter * len, SEEK_END);       // use leesk function
        if (read(wtmpfd, &wtbuf, len) == len) {
            show_info(&wtbuf, len);
        }

    }
    printf("\nwtmp begins ");
    lseek(wtmpfd, 0, SEEK_SET);
    if (read(wtmpfd, &wtbuf, len) == len) {
        showtime1(wtbuf.ut_time);
        printf("\n");
    }

    close(wtmpfd);     // close the tracking
    return 0;

}
// print info user/ststem boot
void show_info(struct utmp* wtbuf, int size) {
    if (wtbuf->ut_type == RUN_LVL) {
        numberOfRecords++;
        return;
    }

    printf("%-8.8s", wtbuf->ut_user);
    printf(" ");
#ifdef SHOWLINE
    if (wtbuf->ut_line[1] != '\0') {
        printf("%-12.12s", wtbuf->ut_line);
    }
    else
        printf("%-12.12s", SYSTEM_BOOT);
#endif
    printf(" ");
#ifdef SHOWHOST
    if (wtbuf->ut_host[0] != '\0')
        printf("%-16.16s", wtbuf->ut_host);
#endif
    printf(" ");
    showtime(wtbuf->ut_time);
    printf("\n");
}


// print time
void showtime(long timeval) {
    char* cp;
    cp = ctime(&timeval);
    printf("%8.16s", cp);
}
void showtime1(long timeval) {
    char* cp;
    cp = ctime(&timeval);
    printf("%8.24s", cp);
}