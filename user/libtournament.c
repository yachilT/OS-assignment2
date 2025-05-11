#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int * locks;

int tournament_create(int processes){
    locks = malloc(sizeof(int*) * (processes-1));
    for (int i = 0; i < processes - 1; i++) {
        locks[i] = peterson_create();
        if (locks[i] == -1)
        return -1;
    }

    for (int i = 0; i < processes - 1; i++) {
        int n = fork();
        if (n == -1)
            return -1;
        
        
        if (n == 0) // if child
            return i; // tournament ID
    }
    return processes-1;


}

int tournament_acquire(void);

int tournament_release(void);
