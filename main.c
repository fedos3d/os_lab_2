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

#include "bsdinfo.h"

//here i put hashmap lib:
#include "utash/uthash.h"
//hashmap ended

//here i create struc of my hashmap for proc info
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

int main()
{
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
    char *curproc = (char*)malloc(13 * sizeof(char));
    char *curname = (char*)malloc(13 * sizeof(char));
    double curcpu;
    int currss;
    int curv_size;
    int curid;
    int curstart;
    int curtt;
    char *curstat = (char*)malloc(13 * sizeof(char));

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
        sprintf(curproc, "Process Name: %s", kp[0].ki_comm);
        add_info(curid, curproc, kp[0].ki_uid, curcpu, curv_size, currss, curtt, curstat, curstart);
        //proc_count++;
    }

    //here I sort my array of procIds;
    int j;
    int a;
    for (i = 0; i < amofproc; ++i)
    {
        for (j = i + 1; j < amofproc; ++j)
        {
            if (procids[i] > procids[j])
            {
                a =  procids[i];
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
}