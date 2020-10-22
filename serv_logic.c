#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/proc.h>
#include <kvm.h>
#include <vm/vm_param.h>
#include <string.h>
#include <sys/user.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#define O_RDONLY 0x0000
#define _POSIX2_LINE_MAX 2048
#define _PATH_DEVNULL "/dev/null"
#define SHSIZE 10000000

//here i put hashmap lib:
#include "utash/uthash.h"
//hashmap ended

//here i create struct of my hashmap for proc info
struct myhash {
    int id;
    char user[256];
    char name[256];
    char stat[20];
    int start;
    int v_size;
    int rss;
    int tt;
    double cpu;
    int info[256];
    UT_hash_handle hh;
};
//here i declare my hashmap
struct myhash *procnfo = NULL;

//here is the method for adding procs to hashmap
void add_info(int user_id, char *name, int user, double cpu, int v_size, int rss, int tt, char *stat, int start) {
    struct myhash *s;
    s = malloc(sizeof(struct myhash));
    s->start = start;
    s->tt = tt;
    s->rss = rss;
    s->v_size = v_size;
    s->id = user_id;
    s->cpu = cpu;
    strcpy(s->stat, stat);
    strcpy(s->name, name);
    struct passwd *pws;
    pws = getpwuid(user);
    strcpy(s->user, pws->pw_name);
    HASH_ADD_INT( procnfo, id, s );  /* id: name of key field */
}

//find proc func
struct myhash *find_proc(int user_id) {
    struct myhash *s;

    HASH_FIND_INT( procnfo, &user_id, s );  /* s: output pointer */
    return s;
}
void* create_shared_memory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibility, -1, 0);
}
/* here are methods for preparing string to be passed to execvp
char* trim(std::string s) {

    int i = 0, j;
    while ((s[i] == ' ') || (s[i] == '\t')) {
        i++;
    }
    i--;

    while (i > -1) {
        s.erase(s.begin() + i);
        i--;
    }

    i = s.length() - 2;
    while((s[i] == ' ') || (s[i] == '\t')) {
        s.erase(s.begin() + i);
        i--;
    }
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());

    return cstr;
}

char** split(char* str) {
    std::vector<char*> message;
    char *istr;
    istr = strtok (str, " ");
    while (istr != NULL) {
        printf("%s", str);
        message.push_back(istr);
        istr = strtok (NULL, " ");
    }
    message[message.size() - 1][strlen(message[message.size() - 1]) - 1] = '\0';
    char **argv = new char* [message.size() + 1];
    for (int i = 0; i < message.size(); i++)
        argv[i] = message[i];
    argv[message.size()] = NULL;
    return argv;
}

char* concat(char *s1, char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char *result = (char*)malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}
*/

int main(int argc, char **argv) {
    //here we choose what prog will be executed
    void *shmem = create_shared_memory(SHSIZE); // how do i choose such a shared memory size?
    //somewhere here we demonize
    int whatprog;
    whatprog = 4;
    pid_t logger = fork();
    int f = open("/home/log.txt", O_WRONLY);
    if (logger == 0) { //here i set up a logger
        setsid();
        int signo;
        sigset_t newmask;
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGUSR1);
        pthread_sigmask(SIG_BLOCK, &newmask, NULL);
        write(f, "Hello, kek\n", 12);
        memcpy(shmem, "\0", 1);
        while (1) {
            sigwait(&newmask, &signo);
            if (signo == SIGUSR1) {
                write(f, shmem, strlen((char *) shmem));
                memcpy(shmem, "\0", 1);
            }
        }
    } else if (logger < 0) {
        exit(1);
    }
    else {
            if (whatprog == 1) { //ps command
                pid_t p;
                int pipe1[2];
                if (pipe(pipe1) == -1) {
                    fprintf(stderr, "Pipe Failed");
                    return 1;
                }
                p = fork(); //here we fork
                if (p < 0) {
                    fprintf(stderr, "fork Failed");
                    return 1;
                } else if (p > 0) {
                    char concat_str[1000000];
                    wait(NULL);
                    read(pipe1[0], concat_str, 1000000);
                    printf("String from pipe1: %s\n", concat_str);
                    memcpy(shmem, concat_str, sizeof(concat_str));
                    kill(logger, SIGUSR1);
                    //write(fd, test, sizeof(test));
                    close(pipe1[0]);
                } else {
                    char send_pipe[1000000];
                    dup2(pipe1[1], 1); //here i redirect to pipe1
                    dup2(pipe1[1], 2);

                    //here my ps code
                    struct myhash *s;
                    int i, proc_count = 0;
                    char *cuname;
                    char buf[_POSIX2_LINE_MAX];
                    const char *execf, *coref;
                    static kvm_t *kd;
                    struct kinfo_proc *kp;
                    static int nproc;
                    static struct kinfo_proc *plist;

                    execf = NULL;
                    coref = _PATH_DEVNULL;
                    kd = kvm_openfiles(execf, coref, NULL, O_RDONLY, buf);
                    plist = kvm_getprocs(kd, KERN_PROC_PROC, 0, &nproc);
                    int amofproc = nproc;
                    int procids[amofproc];
                    char *curproc = (char *) malloc(13 * sizeof(char));
                    char *curname = (char *) malloc(13 * sizeof(char));
                    double curcpu;
                    int currss;
                    int curv_size;
                    int curid;
                    int curstart;
                    int curtt;
                    char *curstat = (char *) malloc(13 * sizeof(char));

                    //here i fill my hashtruct with processes info
                    for (i = 0, kp = plist; i < nproc; i++, kp++) {
                        curid = kp[0].ki_pid;
                        procids[i] = curid;
                        curv_size = kp[0].ki_size;
                        currss = kp[0].ki_rssize;
                        curcpu = kp[0].ki_pctcpu;
                        curtt = kp[0].ki_tpgid;
                        //curstart = kp[0].ki_start;  dont run in order not to get sef fault
                        (curstat, "%s", kp[0].ki_stat);
                        sprintf(curproc, "Name: %s", kp[0].ki_comm);
                        add_info(curid, curproc, kp[0].ki_uid, curcpu, curv_size, currss, curtt, curstat, curstart);
                        //proc_count++;
                    }

                    //here I sort my array of procIds;
                    int j;
                    int a;
                    for (i = 0; i < amofproc; ++i) {
                        for (j = i + 1; j < amofproc; ++j) {
                            if (procids[i] > procids[j]) {
                                a = procids[i];
                                procids[i] = procids[j];
                                procids[j] = a;
                            }
                        }
                    }
                    //sort ended

                    //here i print info about each proc
                    for (i = 0; i < amofproc; i++) {
                        s = find_proc(procids[i]);
                        printf("user: %s, ", s->user); //username
                        printf("PID: %d, ", procids[i]); //process id
                        printf("CPU: %f, ", s->cpu); // seems its not correct
                        // here it must be %mem, but can't find such in kinfo_proc struct
                        printf("VSZ: %d, ", s->v_size / 1024); //vsz...seems fine
                        printf("RSS: %d, ", s->rss * 4); //rss...seems fine
                        printf("TT: %d, ", s->tt); //tt...it gives numbers, but how do i interpret them?
                        //printf("Stat: %s, ", s->stat); //segfault
                        //printf("%d, ", s->start); //start time, need to work with timeval and two time markers
                        // here it must be time, but something went wrong)))
                        printf("%s\n", s->name); //name of the process
                    }
                    exit(0);
                }
            }
            if (whatprog == 3) { //system info ?maybe need fixing?
                pid_t p;
                int pipe1[2];
                if (pipe(pipe1) == -1) {
                    fprintf(stderr, "Pipe Failed");
                    return 1;
                }
                p = fork(); //here we fork
                if (p < 0) {
                    fprintf(stderr, "fork Failed");
                    return 1;
                } else if (p > 0) {
                    char concat_str[1000000];
                    wait(NULL);
                    read(pipe1[0], concat_str, 1000000);
                    printf("String from pipe1: %s\n", concat_str); //change to write to socket
                    close(pipe1[0]);
                } else {
                    dup2(pipe1[1], fileno(stdout)); //here i redirect to pipe1
                    dup2(pipe1[1], fileno(stderr));
                    int nums;
                    char buffer[100];
                    int parameters1[] = {CTL_HW, HW_MACHINE};
                    int parameters2[] = {CTL_HW, HW_MODEL};
                    int parameters3[] = {CTL_HW, HW_MACHINE_ARCH};
                    int parameters4[] = {CTL_HW, HW_PHYSMEM};
                    int parameters5[] = {CTL_HW, HW_PAGESIZE};
                    size_t len = sizeof(buffer);
                    size_t num_len = sizeof(int);
                    sysctl(parameters1, 2, buffer, &len, NULL, 0);
                    printf("Machine type: %s\n", buffer); // change to write
                    sysctl(parameters2, 2, buffer, &len, NULL, 0);
                    printf("Machine model: %s\n", buffer);
                    sysctl(parameters3, 2, buffer, &len, NULL, 0);
                    printf("Machine arch: %s\n", buffer);
                    sysctl(parameters4, 2, &nums, &num_len, NULL, 0);
                    printf("Physmem: %d\n", nums);
                    sysctl(parameters5, 2, &nums, &num_len, NULL, 0);
                    printf("Memory page size %d\n", nums);
                    exit(0);
                }
            }
            if (whatprog == 4) { //here we start any program // don't work yet
                pid_t p;
                int pipe1[2];
                if (pipe(pipe1) == -1) {
                    fprintf(stderr, "Pipe Failed");
                    return 1;
                }
                p = fork(); //here we fork
                if (p < 0) {
                    fprintf(stderr, "fork Failed");
                    return 1;
                } else if (p > 0) {
                    char concat_str[1000000];
                    wait(NULL);
                    read(pipe1[0], concat_str, 1000000);
                    write(1, concat_str, sizeof(concat_str));
                    close(pipe1[0]);
                } else {
                    dup2(pipe1[1], fileno(stdout)); //here i redirect to pipe1
                    dup2(pipe1[1], fileno(stderr));
                    int ifdeamon; // if 1 - nodeamon, if 0 - daemon;
                    int setuserid;
                    setuid(setuserid);
                    char *cmd = "ps";
                    char *argv[3];
                    argv[0] = "ps";
                    argv[1] = "-aux";
                    argv[2] = NULL;
                    if (execvp(cmd, argv) == -1) {
                        exit(-1);
                    }
                    exit(0);
                }
            }
            if (whatprog == 5) { //send signals to any process
                int procid; //process id to kill
                int killsig; //what signal to convey
                if (kill(procid, killsig) == -1) {
                    exit(-1);
                }
                exit(0);
            }
        }
    }
