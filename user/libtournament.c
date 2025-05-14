#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int * locks;
int tournamentId = -1;
int processesNum;

// helper func
int log(int n) {
    int count = 0;
    while(n != 1) {
        count++;
        n = n >> 1;
    }
    return count;
}


int tournament_create(int processes){
    processesNum = processes;
    locks = malloc(sizeof(int) * (processes - 1));
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
            tournamentId = i;
            return i; // tournament ID
        }
    }
    tournamentId = processes - 1;
    return processes-1;


}

int tournament_acquire(void){
    int index = tournamentId;
    int levels = log(processesNum);
    

    for (int l = levels - 1; l >= 0; l--) {
        int role =  (index & (1 << (levels - l - 1))) >> (levels - l - 1);
        int lock = index >> (levels - l);
        int i = lock + (1 << l) - 1;
        if (peterson_acquire(locks[i], role) == -1) {
            return -1;
        }
    }
    return 0;
}

int tournament_release(void) {
    int index = tournamentId;
    int levels = log(processesNum);

    for (int l = 0; l < levels; l++) {
        int role =  (index & (1 << (levels - l - 1))) >> (levels - l - 1);
        int lock = index >> (levels - l);
        int i = lock + (1 << l) - 1;

        if (peterson_release(locks[i], role) == -1){
            return -1;
        }
    }
    return 0;
}
