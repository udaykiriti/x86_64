#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int main() {
    struct utsname u;
    struct sysinfo s;

    uname(&u);
    sysinfo(&s);
    /*
    if (uname(&sysinfo) == -1) {
        perror("uname");
        return 1;
    } */

    printf("System Name : %s\n", u.sysname);
    printf("Node Name   : %s\n", u.nodename);
    printf("Release     : %s\n", u.release);
    printf("Version     : %s\n", u.version);
    printf("Machine     : %s\n", u.machine);

    printf("Tot ram : %lu MB\n",s.totalram/(1024/1024));
    
    printf("Avbl ram : %lu MB\n",s.freeram/(1024/1024));

    return 0;
}
