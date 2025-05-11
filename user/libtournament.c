#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int * locks;
int * pids;
int processesNum;

int tournament_create(int processes){
    processesNum = processes;
    locks = malloc(sizeof(int*) * (processes-1));
    pids = malloc(sizeof(int*) * processes);
    for (int i = 0; i < processes - 1; i++) {
        locks[i] = peterson_create();
        if (locks[i] == -1)
        return -1;
    }

    for (int i = 0; i < processes - 1; i++) {
        int n = fork();
        if (n == -1)
            return -1;
        
        
        if (n == 0) { // if child
            pids[i] = getpid();
            return i; // tournament ID
        }
    }
    pids[processes-1] = getpid();
    return processes-1;


}

int tournament_acquire(void){
    int index = getTournamentID(getpid());

    int levels = log(processesNum);

    for (int l = levels - 1; l >= 0; l--) {
        int role =  (index & (1 << (levels - l - 1))) >> (levels - l - 1);
        int lock = index >> levels - l;

        peterson_acquire(locks[lock], role);
    }

}

int getTournamentID(int pid) {
    for (int i = 0; i < processesNum; i++){
        if (pid == pids[i])
            return i;
    }
    return -1;
}

int log(int n) {
    int count = 0;
    while(n != 1) {
        count++;
        n = n << 1;
    }
    return count;
}


int tournament_release(void) {
    int index = getTournamentID(getpid());
    int levels = log(processesNum);

    for (int l = 0; l < levels; l++) {
        int role =  (index & (1 << (levels - l - 1))) >> (levels - l - 1);
        int lock = index >> levels - l;

        peterson_release(locks[lock], role);
    }
}
